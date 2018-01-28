struct msgbuf{
    long type;
    int id;
    char username[64];
    char password[64];
    char group_name[64];
    char massage[1024];
};

struct user {
	int ID;
	char username[64];
	char password[64];
    int zal;
};
