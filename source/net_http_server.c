/*
    RB3Enhanced - net_http_server.c
    Hacky HTTP server for interacting with the game over the local network.
*/

#ifdef RB3E_XBOX

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "rb3/InetAddress.h"
#include "rb3/SongMetadata.h"
#include "rb3/BandSongMgr.h"
#include "rb3enhanced.h"
#include "version.h"
#include "ports.h"
#include "config.h"
#include "MusicLibrary.h"
#include "net.h"
#include "net_events.h"

#define SERVER_PORT 0x524E // 'RN' for uhhh rocknet or something

int RB3E_HTTPSocket = -1;
static int InitFailed = 0;
static char *PendingShortname = NULL;

typedef struct _song_list_vector
{
    int *start;
    int *end;
    int unk1;
} song_list_vector;
static song_list_vector song_list = {0};

void HTTP_Server_Init()
{
    RB3E_HTTPSocket = RB3E_CreateSocket(RB3E_TYPE_TCP);
    // TODO(Emma): will the socket api ever return another error?
    if (RB3E_HTTPSocket == -1)
    {
        InitFailed = 1;
        return;
    }
    // bind the local port and set it to be non-blocking
    RB3E_SetNonBlocking(RB3E_HTTPSocket, 1);
    RB3E_BindPort(RB3E_HTTPSocket, SERVER_PORT);
    // start listening for new connections
    RB3E_TCP_Listen(RB3E_HTTPSocket);
    RB3E_MSG("HTTP server running!", NULL);
}

typedef enum _HTTP_Request_Status
{
    HTTP_REQUEST_METHOD,
    HTTP_REQUEST_PATH,
    HTTP_REQUEST_VERSION,
    HTTP_REQUEST_HEADER,
    HTTP_REQUEST_DONE
} HTTP_Request_Status;

void HTTP_Server_Accept(void *connection)
{
    int s = (int)connection;
    int conn_time_passed = 0;
    char work_buffer[512] = {0};
    char *work_ptr = work_buffer;
    int req_state = HTTP_REQUEST_METHOD;
    int state_read_bytes = 0;
    int read_bytes = 0;
    char request_path[250] = {0};
    int request_valid = 0;
    char last_byte = '\0';
    const char ok_response[] = "HTTP/1.1 200 OK\r\nServer: RB3Enhanced " RB3E_BUILDTAG "\r\nContent-Type: text/html\r\nContent-Length: 2\r\n\r\nOK";
    const char notfound_response[] = "HTTP/1.1 404 Not Found\r\nServer: RB3Enhanced " RB3E_BUILDTAG "\r\nContent-Length: 9\r\n\r\nNot Found";
    char response_buffer[2000] = {0};
    int song_id = 0;
    char *shortname = NULL;
    SongMetadata *song_metadata = NULL;
    int list_count = 0;
    int i = 0;

    // TODO(Emma): Properly evaluate the security (and stability!) of this.
    while (req_state < HTTP_REQUEST_DONE)
    {
        if (RB3E_TCP_Recv(s, work_ptr, 1) != -1)
        {
            last_byte = *work_ptr;
            // if we've read more than we can chew, back out immediately
            if (state_read_bytes > sizeof(work_buffer))
                goto close_connection;
            // advance the buffers forward
            work_ptr++;
            read_bytes++;
            state_read_bytes++;
        }
        // check if we had an error that wasn't due to blocking, or if the request took too long
        else if (RB3E_LastError() != 10035 || conn_time_passed > 1000)
        {
            goto close_connection;
        }
        // sleep the thread and count up
        else
        {
            RB3E_Sleep(5);
            conn_time_passed += 5;
            continue;
        }
        if (req_state == HTTP_REQUEST_METHOD && last_byte == ' ')
        {
            if (strcmp(work_buffer, "GET ") == 0)
            {
                req_state = HTTP_REQUEST_PATH;
                memset(work_buffer, 0, sizeof(work_buffer));
                work_ptr = work_buffer;
                state_read_bytes = 0;
            }
            else
            {
                RB3E_DEBUG("Got unknown request method '%s', closing connection", work_buffer);
                goto close_connection;
            }
        }
        else if (req_state == HTTP_REQUEST_PATH && last_byte == ' ')
        {
            // null us off
            work_ptr[-1] = '\0';
            strncpy(request_path, work_buffer, sizeof(request_path));
            req_state = HTTP_REQUEST_VERSION;
            memset(work_buffer, 0, sizeof(work_buffer));
            work_ptr = work_buffer;
            state_read_bytes = 0;
        }
        else if (req_state == HTTP_REQUEST_VERSION && memcmp(work_ptr - 2, "\r\n", 2) == 0)
        {
            req_state = HTTP_REQUEST_HEADER;
            memset(work_buffer, 0, sizeof(work_buffer));
            work_ptr = work_buffer;
            state_read_bytes = 0;
        }
        else if (req_state == HTTP_REQUEST_HEADER && memcmp(work_ptr - 2, "\r\n", 2) == 0)
        {
            // TODO(Emma): Validate Origin header
            work_ptr[-1] = '\0';
            work_ptr[-2] = '\0';
            if (state_read_bytes > 2)
            {
                RB3E_DEBUG("Got HTTP header '%s'", work_buffer);
                req_state = HTTP_REQUEST_HEADER;
            }
            else
                req_state = HTTP_REQUEST_DONE;
            memset(work_buffer, 0, sizeof(work_buffer));
            work_ptr = work_buffer;
            state_read_bytes = 0;
        }
    }
    RB3E_DEBUG("Got a request for '%s' over HTTP", request_path);
    if (strstr(request_path, "/song_") == request_path)
    {
        if (sscanf(request_path + sizeof("/song"), "%i", &song_id) == 1)
        {
            RB3E_DEBUG("Fetching song metadata for %i over HTTP", song_id);
            song_metadata = GetMetadata((BandSongMgr *)PORT_THESONGMGR, song_id);
            if (song_metadata == NULL)
            {
                RB3E_TCP_Send(s, (void *)notfound_response, sizeof(notfound_response));
                goto close_connection;
            }
            // TODO(Emma): Good fucking christ what is this
            strcat(response_buffer, "HTTP/1.1 200 OK\r\n");
            strcat(response_buffer, "Server: RB3Enhanced " RB3E_BUILDTAG "\r\n");
            if (config.AllowCORS)
                strcat(response_buffer, "Access-Control-Allow-Origin: *\r\n");
            strcat(response_buffer, "Content-Type: text/plain\r\n");
            strcat(response_buffer, "\r\n");
            strcat(response_buffer, "shortname=");
            strcat(response_buffer, song_metadata->shortname);
            strcat(response_buffer, "\r\n");
            strcat(response_buffer, "title=");
            strcat(response_buffer, song_metadata->title.buf);
            strcat(response_buffer, "\r\n");
            strcat(response_buffer, "artist=");
            strcat(response_buffer, song_metadata->artist.buf);
            strcat(response_buffer, "\r\n");
            strcat(response_buffer, "album=");
            strcat(response_buffer, song_metadata->album.buf);
            strcat(response_buffer, "\r\n");
            strcat(response_buffer, "origin=");
            strcat(response_buffer, song_metadata->gameOrigin);
            strcat(response_buffer, "\r\n");
            strcat(response_buffer, "\r\n");
            RB3E_TCP_Send(s, (void *)response_buffer, strlen(response_buffer));
            goto close_connection;
        }
    }
    else if (strstr(request_path, "/jump?shortname=") == request_path)
    {
        // TODO(Emma): This will crash if you're not in the music library. Figure out if we can detect what screen we're on
        shortname = request_path + 16;
        RB3E_DEBUG("Jumping to song %s in Music Library", shortname);
        // hacky solution to send the shortname to the main thread
        PendingShortname = shortname;
        while (PendingShortname != NULL)
            RB3E_Sleep(1);
        RB3E_TCP_Send(s, (void *)ok_response, strlen(ok_response));
        goto close_connection;
    }
    else if (strcmp(request_path, "/list_songs") == 0)
    {
        if (song_list.start == NULL)
            SongMgrGetRankedSongs((BandSongMgr *)PORT_THESONGMGR, &song_list, 0, 0);
        list_count = ((int)song_list.end - (int)song_list.start) / 4;
        // TODO(Emma): ...This, also, is not very great.
        strcat(response_buffer, "HTTP/1.1 200 OK\r\n");
        strcat(response_buffer, "Server: RB3Enhanced " RB3E_BUILDTAG "\r\n");
        strcat(response_buffer, "Content-Type: text/plain\r\n");
        if (config.AllowCORS)
            strcat(response_buffer, "Access-Control-Allow-Origin: *\r\n");
        strcat(response_buffer, "\r\n");
        // flush our buffer and send each song as its own packet
        RB3E_TCP_Send(s, (void *)response_buffer, strlen(response_buffer));
        memset(response_buffer, 0, sizeof(response_buffer));
        for (i = 0; i < list_count; i++)
        {
            song_id = song_list.start[i];
            song_metadata = GetMetadata((BandSongMgr *)PORT_THESONGMGR, song_id);
            if (song_metadata != NULL)
            {
                strcat(response_buffer, "[");
                strcat(response_buffer, song_metadata->shortname);
                strcat(response_buffer, "]\r\n");
                strcat(response_buffer, "shortname=");
                strcat(response_buffer, song_metadata->shortname);
                strcat(response_buffer, "\r\n");
                strcat(response_buffer, "title=");
                strcat(response_buffer, song_metadata->title.buf);
                strcat(response_buffer, "\r\n");
                strcat(response_buffer, "artist=");
                strcat(response_buffer, song_metadata->artist.buf);
                strcat(response_buffer, "\r\n");
                strcat(response_buffer, "album=");
                strcat(response_buffer, song_metadata->album.buf);
                strcat(response_buffer, "\r\n");
                strcat(response_buffer, "origin=");
                strcat(response_buffer, song_metadata->gameOrigin);
                strcat(response_buffer, "\r\n");
                strcat(response_buffer, "\r\n");
                if (strlen(response_buffer) > 1500)
                {
                    RB3E_TCP_Send(s, (void *)response_buffer, strlen(response_buffer));
                    memset(response_buffer, 0, sizeof(response_buffer));
                    // attempt to somewhat stagger the packets out
                    RB3E_Sleep(1);
                }
            }
        }
        if (strlen(response_buffer) > 0)
        {
            RB3E_TCP_Send(s, (void *)response_buffer, strlen(response_buffer));
        }
        goto close_connection;
    }
    else
    {
        // endpoint not found
        RB3E_TCP_Send(s, (void *)notfound_response, sizeof(notfound_response));
    }
close_connection:
    RB3E_DisposeSocket(s);
}

void HTTP_Server_RunLoop()
{
    int r = 0;
    unsigned int connected_ip = 0;
    unsigned short connected_port = 0;
    // init the HTTP socket if we haven't already
    if (RB3E_HTTPSocket == -1)
    {
        HTTP_Server_Init();
        if (InitFailed)
            return;
    }
    // check if we have a shortname to fuck with
    if (PendingShortname != NULL)
    {
        MusicLibrarySelectSong(PendingShortname);
        PendingShortname = NULL;
    }
    // accept incoming connections
    // TODO(Emma): add some sort of subnet restriction and ratelimit
    r = RB3E_TCP_Accept(RB3E_HTTPSocket, &connected_ip, &connected_port);
    if (r != -1)
    {
        RB3E_MSG("New HTTP connection from %08X:%i", connected_ip, connected_port);
        RB3E_CreateThread(HTTP_Server_Accept, (void *)r, 0x1000);
    }
}

#endif
