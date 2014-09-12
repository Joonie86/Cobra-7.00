#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>	
#include <arpa/inet.h>
#include <netdb.h>

#include "netiso.h"

#ifdef DEBUG
#define DPRINTF printf
#else
#define DPRINTF(...)
#endif

#ifdef unix
#define closesocket close
#define get_network_error() (errno)
#define get_network_error_h() (h_errno)
#endif

#ifdef __CELLOS_LV2__
#define get_network_error() (sys_net_errno)
#define get_network_error_h() (sys_net_h_errno)
#endif

static int connect_to_server(char *server, uint16_t port)
{
	struct sockaddr_in sin;
	unsigned int temp;
	int s;
	
	if ((temp = inet_addr(server)) != (unsigned int)-1) 
	{
		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = temp;
	}
	else {
		struct hostent *hp;
		
		if ((hp = gethostbyname(server)) == NULL) 
		{
			DPRINTF("unknown host %s, %d\n", server, get_network_error_h());
			return -1;
		}
		
		sin.sin_family = hp->h_addrtype;
		memcpy(&sin.sin_addr, hp->h_addr, hp->h_length);
	}
	
	sin.sin_port = htons(port);
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0) 
	{
		DPRINTF("socket() failed (errno=%d)\n", get_network_error());
		return -1;
	}
	
	if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
		DPRINTF("connect() failed (errno=%d)\n", get_network_error());
		closesocket(s);
		return -1;
	}
	
	DPRINTF("connect ok\n");	
	return s;
}

static int64_t open_remote_file(int s, char *path, int *abort_connection)
{
	netiso_open_cmd cmd;
	netiso_open_result res;
	int len;
	
	*abort_connection = 0;
	
	len = strlen(path);	
	memset(&cmd, 0, sizeof(cmd));
	cmd.opcode = BE16(NETISO_CMD_OPEN_FILE);
	cmd.fp_len = BE16(len);
	
	if (send(s, &cmd, sizeof(cmd), 0) != sizeof(cmd))
	{
		DPRINTF("send failed (open_remote_file) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	if (send(s, path, len, 0) != len)
	{
		DPRINTF("send failed (open_remote_file) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	if (recv(s, &res, sizeof(res), MSG_WAITALL) != sizeof(res))
	{
		DPRINTF("recv failed (open_remote_file) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	return BE64(res.file_size);
}

static int read_remote_file(int s, void *buf, uint64_t offset, uint32_t size, int *abort_connection)
{
	netiso_read_file_cmd cmd;
	netiso_read_file_result res;
	
	*abort_connection = 0;
	
	memset(&cmd, 0, sizeof(cmd));
	cmd.opcode = BE16(NETISO_CMD_READ_FILE);
	cmd.offset = BE64(offset);
	cmd.num_bytes = BE32(size);
	
	if (send(s, &cmd, sizeof(cmd), 0) != sizeof(cmd))
	{
		DPRINTF("send failed (read_remote_file) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	if (recv(s, &res, sizeof(res), MSG_WAITALL) != sizeof(res))
	{
		DPRINTF("recv failed (read_remote_file) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	int bytes_read = BE32(res.bytes_read);
	if (bytes_read <= 0)
		return bytes_read;
	
	if (recv(s, buf, bytes_read, MSG_WAITALL) != bytes_read)
	{
		DPRINTF("recv failed (read_remote_file) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	return bytes_read;
}

static int create_remote_file(int s, char *path, int *abort_connection)
{
	netiso_create_cmd cmd;
	netiso_create_result res;
	int len;
	
	*abort_connection = 0;
	
	len = strlen(path);	
	memset(&cmd, 0, sizeof(cmd));
	cmd.opcode = BE16(NETISO_CMD_CREATE_FILE);
	cmd.fp_len = BE16(len);
	
	if (send(s, &cmd, sizeof(cmd), 0) != sizeof(cmd))
	{
		DPRINTF("send failed (create_remote_file) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	if (send(s, path, len, 0) != len)
	{
		DPRINTF("send failed (create_remote_file) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	if (recv(s, &res, sizeof(res), MSG_WAITALL) != sizeof(res))
	{
		DPRINTF("recv failed (create_remote_file) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	return BE32(res.create_result);
}

static int write_remote_file(int s, void *buf, uint32_t size, int *abort_connection)
{
	netiso_write_file_cmd cmd;
	netiso_write_file_result res;
	
	*abort_connection = 0;
	
	if (size == 0)
		return 0;
	
	memset(&cmd, 0, sizeof(cmd));
	cmd.opcode = BE16(NETISO_CMD_WRITE_FILE);
	cmd.num_bytes = BE32(size);
	
	if (send(s, &cmd, sizeof(cmd), 0) != sizeof(cmd))
	{
		DPRINTF("send failed (write_remote_file) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	if (send(s, buf, size, MSG_WAITALL) != size)
	{
		DPRINTF("send failed (write_remote_file) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	if (recv(s, &res, sizeof(res), MSG_WAITALL) != sizeof(res))
	{
		DPRINTF("recv failed (write_remote_file) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	return BE32(res.bytes_written);
}

static int open_remote_dir(int s, char *path, int *abort_connection)
{
	netiso_open_dir_cmd cmd;
	netiso_open_dir_result res;
	int len;
	
	*abort_connection = 0;
	
	len = strlen(path);
	memset(&cmd, 0, sizeof(cmd));
	cmd.opcode = BE16(NETISO_CMD_OPEN_DIR);
	cmd.dp_len = BE16(len);
		
	if (send(s, &cmd, sizeof(cmd), 0) != sizeof(cmd))
	{
		DPRINTF("send failed (open_remote_dir) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	if (send(s, path, len, 0) != len)
	{
		DPRINTF("send failed (open_remote_dir) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	if (recv(s, &res, sizeof(res), MSG_WAITALL) != sizeof(res))
	{
		DPRINTF("recv failed (open_remote_dir) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	return BE32(res.open_result);	
}

static int read_remote_dir_entry(int s, char **name, int *is_directory, int64_t *file_size, int *abort_connection)
{
	netiso_read_dir_entry_cmd cmd;
	netiso_read_dir_entry_result res;
	int len;
	
	*abort_connection = 0;
	
	memset(&cmd, 0, sizeof(cmd));
	cmd.opcode = BE16(NETISO_CMD_READ_DIR_ENTRY);
	
	if (send(s, &cmd, sizeof(cmd), 0) != sizeof(cmd))
	{
		DPRINTF("send failed (read_remote_dir_entry) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	if (recv(s, &res, sizeof(res), MSG_WAITALL) != sizeof(res))
	{
		DPRINTF("recv failed (read_remote_dir_entry) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	*file_size = BE64(res.file_size);
	*is_directory = res.is_directory;
	len = BE16(res.fn_len);
	
	if (*file_size >= 0)
	{
		*name = malloc(len+1);
		(*name)[len] = 0;		
		
		if (recv(s, *name, len, MSG_WAITALL) != len)
		{
			DPRINTF("recv failed (read_remote_dir_entry) (errno=%d)!\n", get_network_error());
			*abort_connection = 1;
			free(*name);
			return -1;
		}		
	}
	
	return (*file_size >= 0) ? 0 : -1;
}

static int read_remote_dir_entry_v2(int s, char **name, int *is_directory, int64_t *file_size, uint64_t *mtime, uint64_t *ctime, uint64_t *atime, int *abort_connection)
{
	netiso_read_dir_entry_cmd cmd;
	netiso_read_dir_entry_result_v2 res;
	int len;
	
	*abort_connection = 0;
	
	memset(&cmd, 0, sizeof(cmd));
	cmd.opcode = BE16(NETISO_CMD_READ_DIR_ENTRY_V2);
	
	if (send(s, &cmd, sizeof(cmd), 0) != sizeof(cmd))
	{
		DPRINTF("send failed (read_remote_dir_entry) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	if (recv(s, &res, sizeof(res), MSG_WAITALL) != sizeof(res))
	{
		DPRINTF("recv failed (read_remote_dir_entry) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	*file_size = BE64(res.file_size);
	*is_directory = res.is_directory;
	*mtime = BE64(res.mtime);
	*ctime = BE64(res.ctime);
	*atime = BE64(res.atime);
	len = BE16(res.fn_len);
	
	if (*file_size >= 0)
	{
		*name = malloc(len+1);
		(*name)[len] = 0;		
		
		if (recv(s, *name, len, MSG_WAITALL) != len)
		{
			DPRINTF("recv failed (read_remote_dir_entry) (errno=%d)!\n", get_network_error());
			*abort_connection = 1;
			free(*name);
			return -1;
		}		
	}
	
	return (*file_size >= 0) ? 0 : -1;
}

static int delete_remote_file(int s, char *path, int *abort_connection)
{
	netiso_delete_file_cmd cmd;
	netiso_delete_file_result res;
	int len;
	
	*abort_connection = 0;
	
	len = strlen(path);
	memset(&cmd, 0, sizeof(cmd));
	cmd.opcode = BE16(NETISO_CMD_DELETE_FILE);
	cmd.fp_len = BE16(len);
		
	if (send(s, &cmd, sizeof(cmd), 0) != sizeof(cmd))
	{
		DPRINTF("send failed (delete_remote_file) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	if (send(s, path, len, 0) != len)
	{
		DPRINTF("send failed (delete_remote_file) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	if (recv(s, &res, sizeof(res), MSG_WAITALL) != sizeof(res))
	{
		DPRINTF("recv failed (delete_remote_file) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	return BE32(res.delete_result);	
}

static int delete_remote_dir(int s, char *path, int *abort_connection)
{
	netiso_rmdir_cmd cmd;
	netiso_rmdir_result res;
	int len;
	
	*abort_connection = 0;
	
	len = strlen(path);
	memset(&cmd, 0, sizeof(cmd));
	cmd.opcode = BE16(NETISO_CMD_RMDIR);
	cmd.dp_len = BE16(len);
		
	if (send(s, &cmd, sizeof(cmd), 0) != sizeof(cmd))
	{
		DPRINTF("send failed (delete_remote_dir) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	if (send(s, path, len, 0) != len)
	{
		DPRINTF("send failed (delete_remote_dir) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	if (recv(s, &res, sizeof(res), MSG_WAITALL) != sizeof(res))
	{
		DPRINTF("recv failed (delete_remote_dir) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	return BE32(res.rmdir_result);	
}

static int remote_mkdir(int s, char *path, int *abort_connection)
{
	netiso_mkdir_cmd cmd;
	netiso_mkdir_result res;
	int len;
	
	*abort_connection = 0;
	
	len = strlen(path);
	memset(&cmd, 0, sizeof(cmd));
	cmd.opcode = BE16(NETISO_CMD_MKDIR);
	cmd.dp_len = BE16(len);
		
	if (send(s, &cmd, sizeof(cmd), 0) != sizeof(cmd))
	{
		DPRINTF("send failed (remote_mkdir) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	if (send(s, path, len, 0) != len)
	{
		DPRINTF("send failed (remote_mkdir) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	if (recv(s, &res, sizeof(res), MSG_WAITALL) != sizeof(res))
	{
		DPRINTF("recv failed (remote_mkdir) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	return BE32(res.mkdir_result);	
}

static int remote_stat(int s, char *path, int *is_directory, int64_t *file_size, uint64_t *mtime, uint64_t *ctime, uint64_t *atime, int *abort_connection)
{
	netiso_stat_cmd cmd;
	netiso_stat_result res;
	int len;
	
	*abort_connection = 0;
	
	len = strlen(path);
	memset(&cmd, 0, sizeof(cmd));
	cmd.opcode = BE16(NETISO_CMD_STAT_FILE);
	cmd.fp_len = BE16(len);
		
	if (send(s, &cmd, sizeof(cmd), 0) != sizeof(cmd))
	{
		DPRINTF("send failed (remote_stat) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	if (send(s, path, len, 0) != len)
	{
		DPRINTF("send failed (remote_stat) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	if (recv(s, &res, sizeof(res), MSG_WAITALL) != sizeof(res))
	{
		DPRINTF("recv failed (remote_stat) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	*file_size = BE64(res.file_size);
	if (*file_size == -1)
		return -1;
	
	*is_directory = res.is_directory;
	*mtime = BE64(res.mtime);
	*ctime = BE64(res.ctime);
	*atime = BE64(res.atime);
	
	return 0;	
}

static int64_t remote_get_dir_size(int s, char *path, int *abort_connection)
{
	netiso_get_dir_size_cmd cmd;
	netiso_get_dir_size_result res;
	int len;
	
	*abort_connection = 0;
	
	len = strlen(path);
	memset(&cmd, 0, sizeof(cmd));
	cmd.opcode = BE16(NETISO_CMD_GET_DIR_SIZE);
	cmd.dp_len = BE16(len);
		
	if (send(s, &cmd, sizeof(cmd), 0) != sizeof(cmd))
	{
		DPRINTF("send failed (remote_get_dir_size) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	if (send(s, path, len, 0) != len)
	{
		DPRINTF("send failed (remote_get_dir_size) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	if (recv(s, &res, sizeof(res), MSG_WAITALL) != sizeof(res))
	{
		DPRINTF("recv failed (remote_get_dir_size) (errno=%d)!\n", get_network_error());
		*abort_connection = 1;
		return -1;
	}
	
	return BE64(res.dir_size);	
}

// Samples functions
static int copy_from_remote_file(int s, char *src, char *dst, int *abort_connection)
{
	int64_t file_size;
	uint64_t offset = 0;
	static uint8_t buf[0x4133f];
	FILE *f;
	
	file_size = open_remote_file(s, src, abort_connection);
	if (file_size < 0)
	{
		if (*abort_connection)
		{
			DPRINTF("Connection error opening remote file.\n");
		}
		else
		{
			DPRINTF("Remote file %s doesnt' exist.\n", src);
		}
		
		return -1;
	}
	
	f = fopen(dst, "wb");
	if (!f)
	{
		DPRINTF("Error opening local file: %s\n", dst);
		return -1;
	}
	
	DPRINTF("Remote file opened. Size: %lld bytes\n", (long long int)file_size);
	while (1)
	{
		int bytes_read = read_remote_file(s, buf, offset, sizeof(buf), abort_connection);
		if (bytes_read < 0)
		{
			if (*abort_connection)
			{
				DPRINTF("Connection error reading remote file.\n");
			}
			else
			{
				DPRINTF("Error reading remote file.\n");
			}
			
			fclose(f);
			return bytes_read;
		}
		
		fwrite(buf, 1, bytes_read, f);
		
		if (bytes_read < sizeof(buf))
			break;
		
		offset += bytes_read;
		
		if (offset >= file_size)
			break;
	}
	
	fclose(f);
	// Closing remote file is just a matter of opening an inexistent one
	// Note that debug messages complaining will appear on debug server!
	open_remote_file(s, "/CLOSE", abort_connection);
	if (*abort_connection)
		return -1;
	
	return 0;
}

static int copy_to_remote_file(int s, char *src, char *dst, int *abort_connection)
{
	static uint8_t buf[65536];
	FILE *f;
	
	*abort_connection = 0;
	f = fopen(src, "rb");
	if (!f)
	{
		DPRINTF("Error opening local file: %s\n", src);
		return -1;
	}
	
	if (create_remote_file(s, dst, abort_connection) < 0)
	{
		if (*abort_connection)
		{
			DPRINTF("Connection error creating remote file.\n");
		}
		else
		{
			DPRINTF("Cannot create remote file %s.\n", dst);
		}
		
		return -1;
	}
	
	while (1)
	{
		int bytes_read = fread(buf, 1, sizeof(buf), f);
		
		if (bytes_read <= 0)
			break;
		
		int bytes_written = write_remote_file(s, buf, bytes_read, abort_connection);
		if (bytes_written < 0)
		{
			if (*abort_connection)
			{
				DPRINTF("Connection error writing remote file.\n");
			}
			else
			{
				DPRINTF("Error reading writing file.\n");
			}
			
			fclose(f);
			return -1;
		}
		else if (bytes_written != bytes_read)
		{
			DPRINTF("Number of bytes written smaller than expected. Out of free space in server?\n");
			fclose(f);
			return -1;
		}
		
		if (bytes_read < sizeof(buf))
			break;
	}
	
	fclose(f);
	// Closing remote file is just a matter of creating a new one impossible to create
	// Note that debug messages complaining will appear on debug server!
	create_remote_file(s, "////", abort_connection);
	if (*abort_connection)
		return -1;
	
	return 0;
}

static int list_remote_dir(int s, char *dir, int *abort_connection)
{
	char *name;
	int is_dir;
	int64_t file_size;
	uint64_t mtime, ctime, atime;
	
	if (open_remote_dir(s, dir, abort_connection) < 0)
	{
		if (*abort_connection)
		{
			DPRINTF("Connection error while opening remote directory.\n");			
		}
		else
		{
			DPRINTF("Remote directory %s cannot be opened.\n", dir);
		}
		
		return -1;
	}
	
	while (read_remote_dir_entry_v2(s, &name, &is_dir, &file_size, &mtime, &ctime, &atime, abort_connection) == 0)
	{
		if (is_dir)
		{
			DPRINTF("%s <DIR> (MTIME: %x)\n", name, (uint32_t)mtime);
		}
		else
		{
			DPRINTF("%s <FILE> <%lld KB> (MTIME: %x)\n", name, (long long int)file_size/1024, (uint32_t)mtime);
		}
		
		// Do not forget to free mem
		free(name);
	}
	
	if (*abort_connection)
	{
		DPRINTF("Connection error while reading remote directory.\n");
		return -1;
	}
	
	// No need to close directory
	
	return 0;
}

static int delete_remote_dir_recursive(int s, char *dir, int *abort_connection)
{
	char *name;
	int is_dir;
	int64_t file_size;
		
	if (open_remote_dir(s, dir, abort_connection) < 0)
	{
		if (*abort_connection)
		{
			DPRINTF("Connection error while opening remote directory.\n");			
		}
		else
		{
			DPRINTF("Remote directory %s cannot be opened.\n", dir);
		}
		
		return -1;
	}
	
	while (read_remote_dir_entry(s, &name, &is_dir, &file_size, abort_connection) == 0)
	{
		int ret;
		
		char *path = malloc(strlen(dir)+strlen(name)+2);
		sprintf(path, "%s/%s", dir, name);	
		
		// Do not forget to free mem
		free(name);
		
		if (is_dir)
		{
			ret = delete_remote_dir_recursive(s, path, abort_connection);
			if (ret < 0)
				return ret;
			
			// Original directory has to be opened again!
			if (open_remote_dir(s, dir, abort_connection) < 0)
			{
				if (*abort_connection)
				{
					DPRINTF("Connection error while opening remote directory.\n");			
				}
				else
				{
					DPRINTF("Remote directory %s cannot be opened.\n", dir);
				}
		
				return -1;
			}
		}
		else
		{
			ret = delete_remote_file(s, path, abort_connection);
			if (ret < 0)
			{
				if (*abort_connection)
				{
					DPRINTF("Connection error deleting remote file.\n");
				}
				else
				{
					DPRINTF("Remote file %s cannot be deleted!\n", path);
				}
				
				return ret;
			}
		}
		
		// Do not forget to free mem
		free(path);
	}
	
	if (*abort_connection)
	{
		DPRINTF("Connection error while reading remote directory.\n");
		return -1;
	}
	
	// Now delete directory
	int ret = delete_remote_dir(s, dir, abort_connection);
	
	if (*abort_connection)
	{
		DPRINTF("Connection error while deleting remote directory.\n");
		return -1;
	}
	
	if (ret < 0)
	{
		DPRINTF("Cannot remove remote directory %s\n", dir);
	}
	
	return ret;
}

int main(int argc, char *argv[])
{
	int s;
	int abort_connection;
	uint64_t mtime, ctime, atime;
	int64_t file_size;
	int is_directory;
	
	s = connect_to_server("127.0.0.1", NETISO_PORT);
	if (s < 0)
	{
		printf("Connection with server failed.\n");
		return -1;
	}
	
	if (copy_from_remote_file(s, "/***PS3***/GAMES/BLES00635", "tequen.iso", &abort_connection) < 0)
	{
		if (abort_connection)
		{
			DPRINTF("Aborting connection!\n");
			shutdown(s, SHUT_RDWR);
			closesocket(s);
			return -1;
		}
		
		DPRINTF("Copy from remote failed!\n");
	}
	else
	{
		DPRINTF("Copy from remote success!\n");
	}
	
	if (copy_to_remote_file(s, "/media/isos/DF01.iso", "/PS2ISO/df.iso", &abort_connection) < 0)
	{
		if (abort_connection)
		{
			DPRINTF("Aborting connection!\n");
			shutdown(s, SHUT_RDWR);
			closesocket(s);
			return -1;
		}
		
		DPRINTF("Copy to remote failed!\n");
	}
	else
	{
		DPRINTF("Copy to remote success!\n");
	}
	
	if (list_remote_dir(s, "/BDISO", &abort_connection) < 0)
	{		
		if (abort_connection)
		{
			DPRINTF("Aborting connection!\n");
			shutdown(s, SHUT_RDWR);
			closesocket(s);
			return -1;
		}
		
		DPRINTF("list remote dir failed!\n");
	}
	else
	{
		DPRINTF("list remote dir success!\n");
	}
	
	if (delete_remote_dir_recursive(s, "/killme", &abort_connection) < 0)
	{
		if (abort_connection)
		{
			DPRINTF("Aborting connection!\n");
			shutdown(s, SHUT_RDWR);
			closesocket(s);
			return -1;
		}
		
		DPRINTF("remove remote dir recursive failed!\n");
	}
	else
	{
		DPRINTF("remove remote dir recursive success!\n");
	}
	
	if (remote_mkdir(s, "/foo", &abort_connection) < 0)
	{
		if (abort_connection)
		{
			DPRINTF("Aborting connection!\n");
			shutdown(s, SHUT_RDWR);
			closesocket(s);
			return -1;
		}
		
		DPRINTF("remote mkdir failed (already exists?)!\n");
	}
	else
	{
		DPRINTF("remote mkdir success!\n");
	}
	
	if (remote_stat(s, "/text.txt", &is_directory, &file_size, &mtime, &ctime, &atime, &abort_connection) < 0)
	{
		if (abort_connection)
		{
			DPRINTF("Aborting connection!\n");
			shutdown(s, SHUT_RDWR);
			closesocket(s);
			return -1;
		}
		
		DPRINTF("Cannot stat remote file, probably doesn't exist.\n");
	}
	else
	{
		DPRINTF("Stat: file size: %lld, dir: %d, mtime: %llx\n", (long long int)file_size, is_directory, (long long int)mtime);
	}
	
	file_size = remote_get_dir_size(s, "/PS3ISO", &abort_connection);
	if (file_size < 0)
	{
		if (abort_connection)
		{
			DPRINTF("Aborting connection!\n");
			shutdown(s, SHUT_RDWR);
			closesocket(s);
			return -1;
		}
		
		DPRINTF("remote_get_dir_size failed..\n");
	}
	else
	{
		DPRINTF("Directory size is %lld GB\n", (long long int)file_size/(1024*1024*1024));
	}
	
	shutdown(s, SHUT_RDWR);
	closesocket(s);
	return 0;
}

