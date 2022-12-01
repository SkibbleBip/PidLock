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

#include "../pidLock.hpp"
#include <iostream>
#include <string.h>


int main(void)
{		
	int lock1_status, lock2_status, lock3_status;
	PidLock *lock1, *lock2, *lock3;
	
	std::cout << "Usage: Demonstrates the PID locking capabilities of the PidLock library in C++ programs" << std::endl;
	
	
	try{
		//these functions may throw an exception
		
		lock1 = new PidLock("example.pid");
		lock2 = new PidLock("example.pid");
		lock3 = new PidLock("example2.pid");
		//create the locked objects
		
		lock1_status = lock1->lock();
		lock2_status = lock2->lock();
		lock3_status = lock3->lock();
		//attempt to perform locks on them
		
		
	}catch(PidLockException &ex){
		//in the event an error occurs, catch it
		std::cout << "Failed to process lock: " << ex.what() << std::endl;
		return 1;
	}
	
	//check status of first lockfile
	if(lock1_status > 0){
		std::cout << "Successfully locked first file (Correct)" << std::endl;
	}
	else{
		std::cout << "First file was already locked (Incorrect)" << std::endl;
	}
	//check status of second lock file. this will fail because the lock is already claimed by the first lock object
	if(lock2_status > 0){
		std::cout << "Successfully locked second file (Incorrect)" << std::endl;
	}
	else{
		std::cout << "First file was already locked (Correct)" << std::endl;
	}
	//check status of third locked file
	if(lock3_status > 0){
		std::cout << "Successfully locked third file (Correct)" << std::endl;
	}
	else{
		std::cout << "Third file was already locked (Incorrect)" << std::endl;
	}
	
	//try to relock the third lock. this will fail because it's already locked.
	//notice how trylock does not need to be checked for exceptions
	lock3_status = lock3->trylock();
	if(lock3_status > 0){
		std::cout << "Successfully re-locked third file (Incorrect)" << std::endl;
	}
	else if(lock3_status == 0){
		std::cout << "Third file was already locked (Correct)" << std::endl;
	}
	else{
		std::cout << "Failed to lock third file: " << strerror(-lock3_status);
		delete lock1;
		delete lock2;
		delete lock3;
		//destroy the locked objects
		return 1;
	}
	
	lock3_status = lock3->unlock();
	//unlock the third lockfile
	
	if(lock3_status < 0){
		std::cout << "Failed to unlock third file: " << strerror(-lock3_status);
		delete lock1;
		delete lock2;
		delete lock3;
		//destroy the locked objects
		return 1;
	}
	else{
		std::cout << "Successfully unlocked third file (Correct)" << std::endl;
	}
	
	
	
	lock3_status = lock3->trylock();
	//lock the file again
	if(lock3_status > 0){
		std::cout << "Successfully re-locked third file (Correct)" << std::endl;
	}
	else if(lock3_status == 0){
		std::cout << "Third file was already locked (Incorrect)" << std::endl;
	}
	else{
		std::cout << "Failed to lock third file: " << strerror(-lock3_status);
		delete lock1;
		delete lock2;
		delete lock3;
		//destroy the locked objects
		return 1;
	}
	
	
	
	
	delete lock1;
	delete lock2;
	delete lock3;
	//destroy the locked objects
	//these will unlock and remove any locked files
	
	return 0;
}

