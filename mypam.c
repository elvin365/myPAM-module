#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#define PAM_SM_AUTH
#define MAX_V 30
/* expected hook */
PAM_EXTERN int pam_sm_setcred( pam_handle_t *pamh, int flags, int argc, const char **argv ) {
	return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv) {
	printf("Acct mgmt\n");
	return PAM_SUCCESS;
}

/* expected hook, this is where custom stuff happens */
PAM_EXTERN int pam_sm_authenticate(pam_handle_t * pamh, int flags
                                   ,int argc, const char **argv)
{
        unsigned int ctrl;
        int retval;
        const char *name, *p;
        char *right;
        /*special variables*/
        long x1,x2,x3,x4,y;

        time_t mytime;
        struct tm *mytm;

        /*готовимся к аутентификации*/
        mytime=time(0);
        mytm=localtime(&mytime);


        srandom(mytime);
        x1=random()%MAX_V;
        x2=random()%MAX_V;
        x3=random()%MAX_V;
        x4=random()%MAX_V;

        //завели несколько случайных величин, а заодно узнали и время.

        /* получим имя пользователя */

        // Получаем имя пользователя
        // Вся мудрость PAM в том, что приглашение "login: " появится если имяеще не известно,
        // иначе мы сразу получим ответ, сгенерированный предыдущими модулями.

        retval = pam_get_user(pamh, &name, "login1: ");

        /*получим пароль используя диалог*/

        {
            struct pam_conv *conv;
            struct pam_message *pmsg[3],msg[3];
            struct pam_response *response;


        retval = pam_get_item( pamh, PAM_CONV, (const void **) &conv ) ;

// Сами мы не знаем как будет осущестляться диалог, это забота программы(в нашем случае этим займется login). Мы
// лишь только  укажем параметры, вид приглашения и более того, можемзадать сразу несколько приглашений, если надо
// получить сразу несколько ответов

        pmsg[0] = &msg[0];
        msg[0].msg_style = PAM_PROMPT_ECHO_OFF;
        msg[0].msg=malloc(100);
        //snprintf(msg[0].msg,60,"Second Password:%d:%d:%d:%d:",x1,x2,x3,x4);
	snprintf(msg[0].msg,99,"Enter hour + day of month:");

        retval = conv->conv(1, ( const struct pam_message ** ) pmsg
                            , &response, conv->appdata_ptr);
 // Нам дали указатель на диалоговую функцию. ╢е и запускаем.
        /*просчитаем правильный ответ*/
        //y=2*x1*mytm->tm_mday+x3*mytm->tm_hour;
	y=mytm->tm_mday+mytm->tm_hour;   
	right=malloc(100);
        snprintf(right,20,"%ld",y);

	/*memset(&msg[0],0,60);


	pmsg[0] = &msg[0];
        msg[0].msg_style = PAM_PROMPT_ECHO_OFF;
        msg[0].msg=malloc(100);
        snprintf(msg[0].msg,60,"\nAnswer:%s",right);

        retval = conv->conv(1, ( const struct pam_message ** ) pmsg
                            , &response, conv->appdata_ptr);*/





	
	
 // Сравним с ответом пользователя. Ответ формируется диалоговой функцией в специальном формате.
        if (!(strcmp(right,response->resp))){
        //if ((strcmp(right,response->resp))){
        return PAM_SUCCESS;
        }else{
        return PAM_AUTH_ERR;
        }
      }/*диалог*/
        return PAM_SUCCESS;
// Нашим результатом будет да или нет. Как прервать программу разберется основной модуль PAM.
}
