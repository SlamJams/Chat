#include <iostream>
#include <string>
#include <fstream>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <limits.h>
#include <cstring>
#include <cstdlib>

using namespace std;

int main()
{
	int port_num;
	int sock, n;
	struct sockaddr_in s_address;
	struct hostent *server;
	const int MAXHOSTNAME = 256;
	char buffer[256];
	string s_name;
	char host_name[MAXHOSTNAME];
	string host;

	cout << "Enter a server host name:" << endl;
	getline(cin, host);

	sprintf(host_name, "%.4s", host.c_str());

	gethostname(host_name, MAXHOSTNAME);


	while (host != "program.cs.uh.edu")
	{
		cout << "Not the right server name, try again." << endl;
		cin >> host;
		cin.ignore(100, '\n');
	}

	sprintf(host_name, "%.4s", host.c_str());
	memset(&s_address, 0, sizeof(struct sockaddr_in));
	server = gethostbyname(host.c_str());

	cout << "Enter server port number." << endl;
	cin >> port_num;
	cin.ignore(100, '\n');

	s_address.sin_family = server->h_addrtype;
	s_address.sin_port = htons(port_num);

	while (1)
	{
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) /* create socket */
		{
			cout << "Error connecting to socket." << endl;
			return (-1);
		}

		if (connect(sock, (struct sockaddr *) &s_address, sizeof(struct sockaddr_in)) < 0)
		{
			cout << "error connecting to socket." << endl;
			return (-1);
		}

		cout << "Enter the name of the student to get their grade." << endl;
		//cin >> s_name;
		cin.clear();
		getline(cin, s_name, '\n');


		if (s_name.empty())
		{
			break;
		}


		n = write(sock, s_name.c_str(), s_name.length());
		if (n < 0)
			cout << "error reading from the socket." << endl;

		bzero(buffer, 256);
		n = read(sock, buffer, 255);
		string str(buffer);

		cout << "Student " << s_name << " got a " << str << " on the quiz." << endl;

		close(sock);
	}


	return 0;
}
