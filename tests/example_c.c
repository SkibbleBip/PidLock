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
#include <stdio.h>
#include <string.h>

int main(void)
{
	PidLock *lock1, *lock2, *lock3;
	//create 3 locks
	int lock1_status, lock2_status, lock3_status;
	//create the 3 lock return values
	
	puts("Usage: Demonstrates the PID locking capabilities of the PidLock library in C programs");
	
	lock1 = pidLock_init("example.pid");
	lock2 = pidLock_init("example.pid");
	//initialize 2 locks as the same PID file
	
	lock3 = pidLock_init("example2.pid");
	//initialize the third one as different
	
	if(!(lock1 && lock2 && lock3)){
		puts("Failed to initialize one of the locks:");
		printf("\tlock1: %s lock2: %s lock3: %s\n", 
			   (lock1 ? "success" : "failed"), (lock2 ? "success" : "failed"), (lock3 ? "success" : "failed")
			  );
		return 1;
	}
	
	lock1_status = pidLock_lock(lock1);
	lock2_status = pidLock_lock(lock2);
	lock3_status = pidLock_lock(lock3);
	//perform locks on all 3 lock files
	
	
	if(lock1_status > 0){
		puts("lock1 was successfully locked (Correct)");
	}
	else if(lock1_status == 0){
		puts("lock1 was already locked (Incorrect)");
	}
	else{
		printf("lock1 failed to attempt locking: %s\n", strerror(-lock1_status) );
	}
	//first lock file should lock successfully, as there are no current locked PID files
	
	if(lock2_status > 0){
		puts("lock2 was successfully locked (Incorrect)");
	}
	else if(lock2_status == 0){
		puts("lock2 was already locked (Correct)");
	}
	else{
		printf("lock2 failed to attempt locking: %s\n", strerror(-lock2_status) );
	}
	//second lock file SHOULD fail, as "example.pid" is already claimed by the first lock
	
	lock1_status = pidLock_lock(lock1);
	//lock first lock AGAIN
	
	if(lock1_status > 0){
		puts("lock1 was successfully locked (Incorrect)");
	}
	else if(lock1_status == 0){
		puts("lock1 was already locked (Correct)");
	}
	else{
		printf("lock1 failed to attempt locking: %s\n", strerror(-lock1_status) );
	}
	//try to lock the first lock file a second time, it should fail since it's already locked
	
	if(lock3_status > 0){
		puts("lock3 was successfully locked (Correct)");
	}
	else if(lock3_status == 0){
		puts("lock3 was already locked (Incorrect)");
	}
	else{
		printf("lock3 failed to attempt locking: %s\n", strerror(-lock3_status) );
	}
	//third lock will lock, as example2.pid is not claimed by anyone
	
	
	
	lock3_status = pidLock_unlock(lock3);
	if(lock3_status > 0){
		puts("Successfully unlocked lock3 (Correct)");
	}
	else{
		puts("Failed to unlock lock3 (Incorrect)");
	}
	//unlock the third lock
	
	lock3_status = pidLock_lock(lock3);
	if(lock3_status > 0){
		puts("lock3 was successfully re-locked (Correct)");
	}
	else if(lock3_status == 0){
		puts("lock3 was already locked (Incorrect)");
	}
	else{
		printf("lock3 failed to attempt locking: %s\n", strerror(-lock3_status) );
	}
	//re-lock the third lock, to demonstrate if unlocked, relocking is possible.
	
	
	
	
	pidLock_destroy(lock1);
	pidLock_destroy(lock2);
	pidLock_destroy(lock3);
	//destroy the locks. Any previously locked files will be unlocked and removed
	
}