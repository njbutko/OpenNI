/*****************************************************************************
*                                                                            *
*  OpenNI 1.0 Alpha                                                          *
*  Copyright (C) 2010 PrimeSense Ltd.                                        *
*                                                                            *
*  This file is part of OpenNI.                                              *
*                                                                            *
*  OpenNI is free software: you can redistribute it and/or modify            *
*  it under the terms of the GNU Lesser General Public License as published  *
*  by the Free Software Foundation, either version 3 of the License, or      *
*  (at your option) any later version.                                       *
*                                                                            *
*  OpenNI is distributed in the hope that it will be useful,                 *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
*  GNU Lesser General Public License for more details.                       *
*                                                                            *
*  You should have received a copy of the GNU Lesser General Public License  *
*  along with OpenNI. If not, see <http://www.gnu.org/licenses/>.            *
*                                                                            *
*****************************************************************************/




//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include <XnOS.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <XnLog.h>



//---------------------------------------------------------------------------
// Code
//---------------------------------------------------------------------------
XN_C_API XnStatus xnOSCreateThread(XN_THREAD_PROC_PROTO pThreadProc, const XN_THREAD_PARAM pThreadParam, XN_THREAD_HANDLE* pThreadHandle)
{
	XnStatus nRetVal = XN_STATUS_OK;
	
	// Validate the input/output pointers (to make sure none of them is NULL)
	XN_VALIDATE_INPUT_PTR(pThreadProc);
	XN_VALIDATE_OUTPUT_PTR(pThreadHandle);

	// allocate thread handle
	XN_VALIDATE_ALLOC(*pThreadHandle, pthread_t);

	// Create a thread via the OS
	int rc = pthread_create(*pThreadHandle, NULL, pThreadProc, pThreadParam);
	if (rc != 0)
	{
		XN_FREE_AND_NULL(*pThreadHandle);
		return (XN_STATUS_OS_THREAD_CREATION_FAILED);
	}

	// All is good...
	return (XN_STATUS_OK);
}

XN_C_API XnStatus xnOSTerminateThread(XN_THREAD_HANDLE* pThreadHandle)
{
	// Validate the input/output pointers (to make sure none of them is NULL)
	XN_VALIDATE_INPUT_PTR(pThreadHandle);

	// Make sure the actual thread handle isn't NULL
	XN_RET_IF_NULL(*pThreadHandle, XN_STATUS_OS_INVALID_THREAD);
	
	XN_THREAD_HANDLE handle = *pThreadHandle;

	// send the KILL signal to the thread
	if (0 != pthread_cancel(*handle))
	{
		return (XN_STATUS_OS_THREAD_TERMINATION_FAILED);
	}
	
	// Close the handle
	XnStatus nRetVal = xnOSCloseThread(pThreadHandle);
	XN_IS_STATUS_OK(nRetVal);

	// All is good...
	return (XN_STATUS_OK);
}

XN_C_API XnStatus xnOSCloseThread(XN_THREAD_HANDLE* pThreadHandle)
{
	// Validate the input/output pointers (to make sure none of them is NULL)
	XN_VALIDATE_INPUT_PTR(pThreadHandle);

	// Make sure the actual thread handle isn't NULL
	XN_RET_IF_NULL(*pThreadHandle, XN_STATUS_OS_INVALID_THREAD);

	// free handle
	XN_FREE_AND_NULL(*pThreadHandle);

	// All is good...
	return (XN_STATUS_OK);
}

XN_C_API XnStatus xnOSWaitForThreadExit(XN_THREAD_HANDLE ThreadHandle, XnUInt32 nMilliseconds)
{
	int rc = 0;
	
	// Make sure the actual thread handle isn't NULL
	XN_RET_IF_NULL(ThreadHandle, XN_STATUS_OS_INVALID_THREAD);
	
	if (nMilliseconds == XN_WAIT_INFINITE)
	{
		// join via the OS
		void* pReturnValue;
		rc = pthread_join(*ThreadHandle, &pReturnValue);
	}
	else
	{
		// calculate timeout absolute time. First we take current time
		struct timespec time;
		
		// @todo Porting to Mac
		struct timeval tv;
		gettimeofday(&tv, NULL);
		time.tv_sec = tv.tv_sec;
		time.tv_nsec = tv.tv_usec * 1000;
				
		/* // @todo Porting to Mac - commented
		if (0 != clock_gettime(CLOCK_REALTIME, &time))
		{
			return (XN_STATUS_OS_EVENT_SET_FAILED);
		}
		*/
		
		// add timeout to this time
		time.tv_sec += (nMilliseconds / 1000);
		time.tv_nsec += ((nMilliseconds % 1000) * 1000000);
		
		// join via the OS
		void* pReturnValue;
		
		//rc = pthread_timedjoin_np(*ThreadHandle, &pReturnValue, &time); // @todo Porting to Mac - not yes ready
	}
	
	// check for failures
	if (rc == ETIMEDOUT)
	{
		return (XN_STATUS_OS_THREAD_TIMEOUT);
	}
	else if (rc != 0)
	{
		return (XN_STATUS_OS_THREAD_TERMINATION_FAILED);
	}
	
	// All is good...
	return (XN_STATUS_OK);
}

XN_C_API XnStatus xnOSSetThreadPriority(XN_THREAD_HANDLE ThreadHandle, XnThreadPriority nPriority)
{
	// Make sure the actual thread handle isn't NULL
	XN_RET_IF_NULL(ThreadHandle, XN_STATUS_OS_INVALID_THREAD);

	int nPolicy = 0;
	sched_param param;
	int rc = 0;
	
	if (nPriority == XN_PRIORITY_CRITICAL)
	{
		//param.__sched_priority = 5; // @todo Porting to Mac - not yet ready
		nPolicy = SCHED_RR;
	}
	else
	{
		return (XN_STATUS_OS_THREAD_UNSUPPORTED_PRIORITY);
	}
	
	rc = pthread_setschedparam(*ThreadHandle, nPolicy, &param);
	if (rc != 0)
	{
		xnLogWarning(XN_MASK_OS, "Failed to set thread priority: %s", sys_errlist[rc]);
		return (XN_STATUS_OS_THREAD_SET_PRIORITY_FAILED);
	}
	
	return (XN_STATUS_OK);
}

XN_C_API XnStatus xnOSGetCurrentThreadID(XN_THREAD_ID* pThreadID)
{
	// Validate the input/output pointers (to make sure none of them is NULL)
	XN_VALIDATE_OUTPUT_PTR(pThreadID);

	// Get the current thread id
	*pThreadID = pthread_self();

	// All is good...
	return (XN_STATUS_OK);
}

