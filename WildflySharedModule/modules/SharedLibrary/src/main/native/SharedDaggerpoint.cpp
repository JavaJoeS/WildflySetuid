#include <jni.h>
#include <pwd.h>
#include "SharedDaggerpoint.h"
#include  <iostream>

using namespace std;



JNIEXPORT void JNICALL Java_com_daggerpoint_share_SharedDaggerpoint_greetings(JNIEnv *env, jobject OBJ) {
	cout << "Hello World!" << endl;


	struct passwd *pwd = (struct passwd *)getpwnam("wildfly");
	  if(pwd == 0) {
		  cout << "Hello World!   Error getting uid/gid information for user." << endl;
		 return ;
	  }

	  int uid = pwd->pw_uid;
	  int gid = pwd->pw_gid;

	  if(setgid(gid)) {
		  cout << "Hello World!  Error setting gid for user, current user may not have permission." << endl;
		 return;
	  }

	  if(setuid(uid)) {
		  cout << "Hello World! Error setting uid for user, current user may not have permission." << endl;
		 return ;
	  }
		return;
}
