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
#include <sstream>
using namespace std;

struct Student
{
	string name;
	string grade;
};

string findInfo(Student[], string name, int n);

int main()
{
	string line;
	string name;
	fstream file;
	string temp;
	string grade;
	int new_sock, sock, n;
	int index = 0;
	unsigned short port_num;
	const int MAX = 1024;
	Student Students[MAX];
	char buffer[256];
	struct sockaddr_in sa;
	struct hostent *hp;
	const int MAXHOSTNAME = 18;
	char myname[MAXHOSTNAME + 1];

	for (int i = 0; i < MAX; i++)
	{
		Students[i].name = "";
		Students[i].grade = -1;
	}

	cout << "Enter file name." << endl;
	cin >> name;

	file.open(name.c_str());

	if (file.is_open())
		cout << "file is opened." << endl;
	else
	{
		cout << "error opening file." << endl;
		return(-1);
	}
	while (file >> temp)
	{
		Students[index].name = temp;
		file >> Students[index].grade;
		index++;
	}

	file.close();

	cout << "Enter server port number." << endl;
	cin >> port_num;


	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) /* create socket */
	{
		cout << "Error connecting to socket." << endl;
		return (-1);
	}
	cout << "socket created" << endl;
	memset(&sa, 0, sizeof(struct sockaddr_in));
	gethostname(myname, MAXHOSTNAME);
	hp = gethostbyname(myname);
	if (hp == NULL)
	{
		cout << "Error getting host name." << endl;
		return(-1);
	}

	sa.sin_family = hp->h_addrtype;
	sa.sin_addr.s_addr = htons(INADDR_ANY);
	sa.sin_port = htons(port_num);                /* this is our port number */


	if (bind(sock, (struct sockaddr *) &sa, sizeof(struct sockaddr_in)) < 0)
	{
		close(sock);
		cout << "Error binding." << endl;                               /* bind address to socket */
	}

	cout << "now listening for a connection." << endl;
	listen(sock, 4);                               /* max # of queued connects */

	for (;;)
	{

		if ((new_sock = accept(sock, NULL, NULL)) < 0)
		{
			return(-1);
		}

		bzero(buffer, 256);

		n = read(new_sock, buffer, 255); //255 last spot for null term.

		if (n < 0)
		{
			cout << "error reading from client." << endl;
			return(-1);
		}

		name = buffer;

		grade = findInfo(Students, name, index);

		n = write(new_sock, grade.c_str(), grade.length());

		name = "";
	}

	close(new_sock);
	close(sock);
	return 0;
}

string findInfo(Student array[], string name, int n)
{
	int randNum;
	ostringstream ss;
	randNum = rand() % (100 - 55 + 1) + 55;
	ss << randNum;

	for (int i = 0; i < n; i++)
	{
		if (array[i].name == name)
		{
			return array[i].grade;

		}
	}

	return ss.str();
}
