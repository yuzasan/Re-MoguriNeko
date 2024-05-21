/*!
 *	@brief		FPS����N���X
 *	
 */
#pragma once

#include <time.h>
#include <windows.h>

class CFPS {
	//���Ԍv���p
	static LARGE_INTEGER freq;
	static LARGE_INTEGER time_buf;
	static int fps;
	static int m_Setfps;
	static float deltaTime;
public:
	/*!
		@brief	���������s��
		@retval	����
	**/
	static void Init();

	/*!
		@brief	�U�OFPS�ɂȂ�悤�ҋ@����
		@retval	����
	**/
	static void Wait();

	/*!
		@brief	���݂�FPS���擾
		@retval	FPS
	**/
	static int GetFPS(){ return fps;}

	/*!
	@brief	���݂�deltaTime���擾
	@retval	deltaTime
	**/
	static float GetDeltaTime() { return deltaTime; }
	static LONGLONG GetTimeCnt();
	///<summary>
	///<para>FPS��ݒ肵�܂�</para>
	///<para>1�`60�܂Őݒ�ł��܂�</para>
	///</summary>
	static void SetFPS(int fps);

};

class Time {
public:
	static LARGE_INTEGER freq;
	static LARGE_INTEGER time_buf;
	static void Start();
	static float End();
};