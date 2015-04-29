#include <stdio.h>
#include "com_example_lameonandroid_MainActivity.h"
#include "lame.h"

JNIEXPORT jstring JNICALL Java_com_example_lameonandroid_MainActivity_getLameVersion
  (JNIEnv * env , jobject obj ){

	//const char*  CDECL get_lame_version       ( void );
	const char* versionName = get_lame_version();
	return (*env) -> NewStringUTF(env, versionName); //jstring     (*NewStringUTF)(JNIEnv*, const char*);
}


JNIEXPORT void JNICALL Java_com_example_lameonandroid_MainActivity_convert
  (JNIEnv * env, jobject obj, jstring wavPath, jstring mp3Path){


	//init lame
	lame_global_flags* gfp = lame_init();
	lame_set_num_channels(gfp, 2);
	lame_set_in_samplerate(gfp, 44100);
	lame_set_brate(gfp, 128);
	lame_set_mode(gfp,1);
	lame_set_quality(gfp,2);   /* 2=high  5 = medium  7=low */



}
