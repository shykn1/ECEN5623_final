
#include "util.hpp"
int set_interface_attribs(int fd, int speed)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= (CLOCAL | CREAD);   // ignore modem controls
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         //8-bit characters
    tty.c_cflag &= ~PARENB;    // no parity bit
    tty.c_cflag &= ~CSTOPB;   //  only need 1 stop bit
    tty.c_cflag &= ~CRTSCTS;  //  no hardware flowcontrol

    /* setup for non-canonical mode */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    /* fetch bytes as they become available */
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

void print_scheduler(void)
{
   int schedType;

   schedType = sched_getscheduler(getpid());

   switch(schedType)
   {
       case SCHED_FIFO:
           printf("Pthread Policy is SCHED_FIFO\n");
           break;
       case SCHED_OTHER:
           printf("Pthread Policy is SCHED_OTHER\n"); exit(-1);
         break;
       case SCHED_RR:
           printf("Pthread Policy is SCHED_RR\n"); exit(-1);
           break;
       default:
           printf("Pthread Policy is UNKNOWN\n"); exit(-1);
   }
}

double k_to_theta(double k){
	return atan (k) * 180 / PI;
}

double get_error (double k,double b){

	double cut_height = FOV_HEIGHT - FOV_CUT_UPPERBOUND;
	//double pos1 = (-b/k) - (double)(cut_height/2.0f);
	double offset = (cut_height/2.0-b)/k - (double)(FOV_WIDTH/2.0f);
	//double offset = (pos1+pos2)/2.0f;
	double theta = k_to_theta(k);
	if(theta >= 0)
		theta = 90.0f - theta;
	else
		theta = -90.0f -theta;
	theta = -theta;
 	//cout << "b " << b << endl;
	//cout << "k " << k << endl;
	//cout <<"offset " << offset<<endl;
	//cout <<"theta "<<theta<<endl;
	
	//double pos1 = (-b/k) - (double)(cut_height/2.0f);
	//double pos2 = ((double)(FOV_WIDTH-b)/k - (double)(cut_height)/2.0f);
	//return pos1*0.7+pos2*0.3;
	return 0.8*(offset+theta*1.1);
}



