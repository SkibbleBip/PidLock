/**************************************************************************
Copyright (C) 2022 SkibbleBip
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**************************************************************************/


/***************************************************************************
* File:  pidLock.hpp
* Author:  SkibbleBip
***************************************************************************/
#ifndef PID_LOCK_HPP
#define PID_LOCK_HPP

#include <unistd.h>

#ifdef __cplusplus

#include <string>


	class PidLock{
	private:
		int pidFile;
		std::string location;
		pid_t pid;
		bool lockStatus;
		
	public:
		int lock(void);
		int unlock(void);
		int trylock(void);
		pid_t getPID(void);
		const char* getLocation(void);
		PidLock(const char* location);
		PidLock(const std::string location);
		PidLock(const PidLock& pl);
		~PidLock(void) noexcept(false);
		bool isLocked(void);


	};



	 class PidLockException : std::exception{
	 private:
		 int errNum;

	 public:
		 PidLockException(int errNum);
	     const char* what(void) const noexcept;
		 int getErrNum(void){return errNum;}
	 };



#endif // __cplusplus


/*Function definitions for C-type methods. These are used in C programs that
*have been linked to this C++ library
*/

#ifdef __cplusplus
	extern "C" { 
#else
	typedef struct PidLock PidLock;
	typedef PidLock PidLock;
#endif

	int pidLock_lock(PidLock *pidLock);
	int pidLock_unlock(PidLock *pidLock);
	pid_t pidLock_getPID(PidLock *pidLock);
	const char* pidLock_getLocation(PidLock *pidLock);
	PidLock* pidLock_init(const char* location);
	void pidLock_destroy(PidLock *pidLock);
	int pidLock_isLocked(PidLock *pidLock);
	
#ifdef __cplusplus
	}
#endif
	
	

	

#endif //PID_LOCK_HPP
