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
* File:  pidLock.cpp
* Author:  SkibbleBip
* Procedures:
* PidLock       -constructor for creating a lockable PID object via char*
* 					location. Throws PidLockException in the event a failure occurs
* PidLock : PidLock     -constructor for creating a lockable PID object via
* 							char* location. Throws PidLockException in the
* 							event a failure occurs
* lock  -perform a lock attempt. Throws PidLockException if error occured
* trylock       -performs a lock attempt. Unlike it's sibling
* 					int PidLock::lock(void), it does not throw any exception
* 					and instead returns a negative errno
* unlock        -unlocks the PID file
* ~PidLock      -destructor for PID file. Will unlock if locked
* getPID        -return the PID of the program that was written to the PID file
* getLocation   -returns the path of the PID file
* PidLockException      -constructor of the PidLockException
* PidLockException::what        -what error occured in the exception
***************************************************************************/



#include "pidLock.hpp"

#include <string>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>

//extern "C" {
	
	
/***************************************************************************
* PidLock::PidLock(const char* location)
* Author: SkibbleBip
* Date: 11/08/2022
* Description: constructor for creating a lockable PID object via char* location.
* 	Throws PidLockException in the event a failure occurs
*
* Parameters:
*        location    I/P    const char*    location of the PID file
**************************************************************************/
PidLock::PidLock(const char* location)
{
	this->location = location;
	lockStatus = false;
	pid = -1;

	if( (pidFile = open(location, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR | S_IWUSR)) < 0) {
		throw PidLockException(errno);
	}
}

/***************************************************************************
* PidLock::PidLock(const std::string location)
* Author: SkibbleBip
* Date: 11/08/2022
* Description: overloaded constructor for creating a PID lock file, takes std::string as param
*	Throws PidLockException in the event a failure occurs
*
* Parameters:
*        location    I/P    const std::string    location of PID file
**************************************************************************/
PidLock::PidLock(const std::string location)
{
	this->location = location;
}

/***************************************************************************
* int PidLock::lock(void)
* Author: SkibbleBip
* Date: 11/08/2022
* Description: perform a lock attempt.
* 	Throws PidLockException if error occured
*
* Parameters:
*        lock    O/P    int    returns 0 if file already locked, otherwise
*        						returns PID of program if unlocked or previously
*        						locked by the same object
**************************************************************************/
int PidLock::lock(void)
{
	int ret = this->trylock();
	if(ret < 0)
		throw PidLockException(errno);

	lockStatus = !!ret;
	return ret;
}

/***************************************************************************
* int PidLock::trylock(void)
* Author: SkibbleBip
* Date: 11/08/2022
* Description: performs a lock attempt. Unlike it's sibling int PidLock::lock(void),
* 	it does not throw any exception and instead returns a negative errno
*
* Parameters:
*        trylock    O/P    int    returns 0 if locked, 1 if lock was able to be
*        						aquired or already locked by same object,
*        						or a negative errno if an error occured
**************************************************************************/
int PidLock::trylock(void)
{
	if(lockStatus)
		return 0;
	
	if( flock(pidFile, LOCK_EX | LOCK_NB) ){
		if(EWOULDBLOCK == errno){
			lockStatus = false;
			return 0;
		}
		return -errno;
	}

	pid = getpid();
	std::string pidStr = std::to_string(pid);
	if(write(pidFile, pidStr.c_str(), pidStr.length()) < 0){
		return -errno;
	}
	lockStatus = true;
	return pid;
}

/***************************************************************************
* int PidLock::unlock(void)
* Author: SkibbleBip
* Date: 11/08/2022
* Description: unlocks the PID file
*
* Parameters:
*        unlock    O/P    int    returns 1 if successful, otherwise negative
*        							errno
**************************************************************************/
int PidLock::unlock(void)
{
	if( flock(pidFile, LOCK_UN | LOCK_NB) ){
		return -errno;
	}
	lockStatus = false;
	pid = 0;
	return 1;
}

/***************************************************************************
* PidLock::~PidLock(void) noexcept(false)
* Author: SkibbleBip
* Date: 11/08/2022
* Description: destructor for PID file. Will unlock if locked
*
* Parameters:
**************************************************************************/
PidLock::~PidLock(void) noexcept(false)
{
	if(0 != close(pidFile))
		{}//throw PidLockException(errno);
	if(0 != unlink(location.c_str()))
		{}//throw PidLockException(errno);
	//it's generally bad practice to throw in a dtor, so let's instead
	//just fail quietly
}

/***************************************************************************
* pid_t PidLock::getPID(void)
* Author: SkibbleBip
* Date: 11/08/2022
* Description: return the PID of the program that was written to the PID file
*
* Parameters:
*        getPID    O/P    pid_t    PID that was written
**************************************************************************/
pid_t PidLock::getPID(void)
{
	return pid;
}

/***************************************************************************
* const char* PidLock::getLocation(void)
* Author: SkibbleBip
* Date: 11/08/2022
* Description: returns the path of the PID file
*
* Parameters:
*        getLocation    O/P    const char*    location as a c-string
**************************************************************************/
const char* PidLock::getLocation(void)
{
	return location.c_str();
}

/***************************************************************************
* bool PidLock::isLocked(void)
* Author: SkibbleBip
* Date: 11/09/2022
* Description: returns lock status
*
* Parameters:
*        isLocked    O/P    bool    lock status
**************************************************************************/
bool PidLock::isLocked(void)
{
	return lockStatus;
}

/***************************************************************************
* PidLockException::PidLockException(int errNum)
* Author: SkibbleBip
* Date: 11/08/2022
* Description: constructor of the PidLockException
*
* Parameters:
*        errNum    I/P    int    errorno to declare the type of error that occured
**************************************************************************/
PidLockException::PidLockException(int errNum)
{
	this->errNum = errNum;
}

/***************************************************************************
* const char* PidLockException::what(void) const noexcept
* Author: SkibbleBip
* Date: 11/08/2022
* Description: what error occured in the exception
*
* Parameters:
*        what    O/P    const char*    string translated from the error code
**************************************************************************/
const char* PidLockException::what(void) const noexcept
{
	return strerror(errNum);
}
	
//}


extern "C" {
	

	int pidLock_lock(PidLock *pidLock)
	{
		return pidLock->trylock();
	}
	
	int pidLock_unlock(PidLock *pidLock)
	{
		return pidLock->unlock();
	}
	pid_t pidLock_getPID(PidLock *pidLock)
	{
		return pidLock->getPID();
	}
	const char* pidLock_getLocation(PidLock *pidLock)
	{
		return pidLock->getLocation();
	}
	PidLock* pidLock_init(const char* location)
	{
		try {
			PidLock *pl = new PidLock(location);
			return pl;
		}catch(PidLockException & ex){
			errno = ex.getErrNum();
		}
		return NULL;
	}
	void pidLock_destroy(PidLock *pidLock)
	{
		delete pidLock;
	}
	int pidLock_isLocked(PidLock *pidLock)
	{
		return !!pidLock->isLocked();	
	}
	
	


} //extern "C"


