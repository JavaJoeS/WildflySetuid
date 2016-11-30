#include <jni.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include "SharedDaggerpoint.h"
#include  <iostream>
#include <string>
#include <dirent.h>
#include <stdlib.h>
#include <limits.h>       //For PATH_MAX

using namespace std;



class SharedDaggerpoint {
	public :
		struct passwd *pwd;
		int getFood();
		int dirWalker( string dir );
		int processEntity(struct dirent* entity, string incoming);
};

SharedDaggerpoint *daggerPtr;
SharedDaggerpoint daggerBuffer;

int getFood() {

		string path = "/export/apps/wildfly/wildfly-10.0.0.Final/domain/servers";
		daggerPtr = &daggerBuffer;

		daggerPtr->pwd = getpwnam("wildfly");
		if (daggerPtr->pwd == NULL) {
		      cout << "Failed to get uid" << endl;
		 }

		daggerPtr->dirWalker( path );
		return 99;
	}
	int SharedDaggerpoint::dirWalker( string dir ) {

		 DIR* dp;
		 dirent* entity;

		dp = opendir(dir.c_str());
		//cout << "Process directory: " << dir.c_str() << std::endl;

		if  (NULL == dp) {
		       cout << "could not open directory: " << dir.c_str() << std::endl;
		        return -1;
		}

		entity = readdir(dp);
		while(entity != NULL) {
		        daggerPtr->processEntity(entity, dir );
		        entity = readdir(dp);
		}
		 //we finished with the directory so remove it from the path

		closedir(dp);
		return 99;
}
int SharedDaggerpoint::processEntity(struct dirent* entity, string incoming ) {

		string fqdName = string(incoming+"/"+entity->d_name);
		const char *cstr = fqdName.c_str();

		chown(cstr, daggerPtr->pwd->pw_uid, daggerPtr->pwd->pw_gid );

		DIR *dPtr=NULL;
		try {
			dPtr = opendir(cstr);
		} catch (const std::exception& e) {/** eat exception **/}
		if ( dPtr == NULL) {
			//cout << "processEntity ----   GOT FILE --  FILE TO change: " << fqdName << endl;
			return 200;
		} else {
			closedir( dPtr );
			 if(entity->d_name[0] == '.') {
				            return 200;
			 }
			 //cout << "processEntity ----   GOT DIR --  DIRECTORYTO change: " << fqdName << endl;
			daggerPtr->dirWalker( fqdName );
			return 200;
		}


	    //there are some other types
	    //read here http://linux.die.net/man/3/readdir
	    cout << "Not a file or directory: " << entity->d_name << std::endl;
	}


JNIEXPORT void JNICALL Java_com_daggerpoint_share_SharedDaggerpoint_greetings(JNIEnv *env, jobject OBJ) {
	cout << "Hello SAR C++ Controller" << endl;


	struct passwd *pwd = (struct passwd *)getpwnam("wildfly");
	  if(pwd == 0) {
		  cout << "Error getting uid/gid information for user." << endl;
		 return ;
	  }

	  int uid = pwd->pw_uid;
	  int gid = pwd->pw_gid;

	  //chown("/export/apps/wildfly/wildfly-10.0.0.Final/domain/servers/*", pwd->pw_uid, pwd->pw_gid);

	 getFood();

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

