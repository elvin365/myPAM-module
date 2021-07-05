#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <stdio.h>
#include<math.h>
const struct pam_conv conv = {
	misc_conv,
	NULL
};
void credit(void);

int main(int argc, char *argv[]) {
	pam_handle_t* pamh = NULL;
	int retval;
	const char* user = "nobody";

	if(argc != 2) {
		printf("Usage: app [username]\n");
		exit(1);
	}

	user = argv[1];

	retval = pam_start("check_user", user, &conv, &pamh);

	// Are the credentials correct?
	if (retval == PAM_SUCCESS) {
		printf("Credentials accepted.\n");
		retval = pam_authenticate(pamh, 0);
	}

	// Can the accound be used at this time?
	if (retval == PAM_SUCCESS) {
		printf("Account is valid.\n");
		retval = pam_acct_mgmt(pamh, 0);
	}

	// Did everything work?
	if (retval == PAM_SUCCESS) {
		printf("Authenticated\n");
	} else {
		printf("Not Authenticated\n");
	}
	
	credit();
	

	// close PAM (end session)
	if (pam_end(pamh, retval) != PAM_SUCCESS) {
		pamh = NULL;
		printf("check_user: failed to release authenticator\n");
		exit(1);
	}

	return retval == PAM_SUCCESS ? 0 : 1;
}


void credit(void)
{
	int n,y;
        float p,m,s;
        printf("The Sum of credit(in $): ");
        scanf("%d", &n);
        printf("Amount of years: ");
        scanf("%d", &y);
        printf("Percent %%: ");
        scanf("%f", &p);
 
        p = p / 100;
        m = (n * p * pow((1+p),y)) / (12 * (pow(1+p,y) - 1));
        s = m * 12 * y;
        printf("Monthly payments: %.0f $.\nThe whole payment: %.0f $.\n", m, s);
}



