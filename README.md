# PID Lock

This library serves as a simplified PID locking mechanism for applications so instead of implementing the locked PID file yourself, all that is needed is to link the library and create a PID Lock object and everything will be handled internally.

## What is a PID Lock File?
PID (Process Identification) Files are files created and then locked by the Operating System by running applications and daemons to display in userspace that they are active and running. The file is then written to contain the Process Identification Number of the parent application and locked to prevent multiple instances of the same application from running. PID lock files can become stale when their application gets terminated unexpectedly and not given the time to close and delete the file, in which the application should be able to detect and "refresh" a stale PID file.

## Purpose
Implementing a PID Locking mechanism takes an annoyingly large amount of code that is repetitive across projects. This library solves that problem by simplifying down the repetitive into a single object that automatically creates, locks and unlocks when instructed to by the programmer.

## Example
```
//C++
std::PidLock lock("/var/run/app.pid");
//create the lock object

if(lock.tryLock() == 0){
	std::cout << "Error, app already running" << std::endl;
	return;
}
std::cout << "Application is now running" << std::endl;
thinkAboutLife();

lock.unlock();
return;

```
