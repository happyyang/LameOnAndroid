#include <stdio.h>
#include "com_example_lameonandroid_MainActivity.h"
#include "lame.h"
#include <android/log.h>
#define LOG_TAG "System.out.c"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

char* Jstring2CStr(JNIEnv* env, jstring jstr) {
	char* rtn = NULL;
	jclass clsstring = (*env)->FindClass(env, "java/lang/String");
	jstring strencode = (*env)->NewStringUTF(env, "GB2312");
	jmethodID mid = (*env)->GetMethodID(env, clsstring, "getBytes",
			"(Ljava/lang/String;)[B");
	jbyteArray barr = (jbyteArray)(*env)->CallObjectMethod(env, jstr, mid,
			strencode); // String .getByte("GB2312");
	jsize alen = (*env)->GetArrayLength(env, barr);
	jbyte* ba = (*env)->GetByteArrayElements(env, barr, JNI_FALSE);
	if (alen > 0) {
		rtn = (char*) malloc(alen + 1); //"\0"
		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	(*env)->ReleaseByteArrayElements(env, barr, ba, 0); //
	return rtn;
}

JNIEXPORT jstring JNICALL Java_com_example_lameonandroid_MainActivity_getLameVersion
  (JNIEnv * env , jobject obj ){

	//const char*  CDECL get_lame_version       ( void );
	const char* versionName = get_lame_version();
	return (*env) -> NewStringUTF(env, versionName); //jstring     (*NewStringUTF)(JNIEnv*, const char*);
}


JNIEXPORT void JNICALL Java_com_example_lameonandroid_MainActivity_convert
  (JNIEnv * env, jobject obj, jstring jwav, jstring jmp3){

	//init lame
	lame_global_flags* gfp = lame_init();
	lame_set_num_channels(gfp, 2);
	lame_set_in_samplerate(gfp, 44100);
	lame_set_brate(gfp, 128);
	lame_set_mode(gfp,1);
	lame_set_quality(gfp,2);   /* 2=high  5 = medium  7=low */
	// 3. ����MP3�ı��뷽ʽ
	lame_set_VBR(gfp, vbr_default);

	LOGE("init lame finished ...");

	int initStatusCode = lame_init_params(gfp);
	if(initStatusCode >= 0){

		char* cwav = Jstring2CStr(env, jwav);
		char* cmp3 = Jstring2CStr(env, jmp3);

		FILE* fwav = fopen(cwav, "rt");
		FILE* fmp3 = fopen(cmp3, "wb");

		short int wav_buffer[8192*2];//���ݴ�С��
		unsigned char mp3_buffer[8192];

		LOGE("start convert ...");
		int read, write, total;
		do{

			//��һ������ �Ƕ��������ݷ�������
			// �ڶ��� ���� һ�ζ������ݵĳ���
			// ���������� �����ٴ�
			// ���ĸ����� ���ĸ��ļ������
			read = fread(wav_buffer,sizeof(short int)*2,8192,fwav);

			if(read != 0){
				total += read* sizeof(short int)*2;
				LOGE("111 have converted %d , read=%d\n ",total,read);
				publishJavaProgress(env, obj, total);
				write = lame_encode_buffer_interleaved(gfp,wav_buffer,read,mp3_buffer,8192);
				// fwrite��������4������
				// ��һ������ дʲô����
				// �ڶ������� ��һ�� ����д�೤
				// ���������� ��д���ٴ�
				// ���ĸ����� д���ĸ��ļ�����
				fwrite(mp3_buffer,sizeof(unsigned char),write,fmp3);
			}else{
				lame_encode_flush(gfp,mp3_buffer,8192);
			}


		}while(read != 0);

		lame_close(gfp);
		fclose(fwav);
		fclose(fmp3);
		LOGE("convert completed...");
	}

}

void publishJavaProgress(JNIEnv * env, jobject obj, jint progress) {
	// ����java���� ���³���Ľ�����
// 1.�ҵ�java��LameActivity��class
	//  jclass      (*FindClass)(JNIEnv*, const char*);
	jclass clazz = (*env)->FindClass(env, "com/example/lameonandroid/MainActivity");
	if (clazz == 0) {
		LOGI("can't find clazz");
	}
	LOGI(" find clazz");
	//2 �ҵ�class ����ķ�������
//    jmethodID   (*GetMethodID)(JNIEnv*, jclass, const char*, const char*);

	jmethodID methodid = (*env)->GetMethodID(env, clazz, "updateProgress",
			"(I)V");
	if (methodid == 0) {
		LOGI("can't find methodid");
	}
	LOGI(" find methodid");

	//3 .���÷���
//    void        (*CallVoidMethod)(JNIEnv*, jobject, jmethodID, ...);
	(*env)->CallVoidMethod(env, obj, methodid, progress);
}


