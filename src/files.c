/*  2024-02-06 14:00  */
/*
    yast - yet another slotcar timer
	File: files.c -> some file routines of the project

    Copyright (C)  2016 - 2024 Martin Berentsen


    This file is part of yast.

    yast is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    yast is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with yast.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "files.h"
#include "display.h"

// #include "yast_define.h"
// #include "config.h"

// extern struct CONFIG config;	/* announce the config memory */

/* ----------------------------------------------------------------------
int PathExists(char *sPath) 
returns 0 if true, -1 if not
------------------------------------------------------------------------*/
int PathExists(char *sPath) {
    struct stat st_buf;
    
    int status = stat (sPath, &st_buf);
    
    if (status != 0) {
		fprintf(stderr, "PathExists: unknown path %s\n", sPath );
        return -1;
    }
    
    if (S_ISDIR (st_buf.st_mode)) {
		return 0;
    }

    return -1;
}

/* ----------------------------------------------------------------------
int FileExists(char *sPath, char *sName), sName will be attached to sPath
returns 0 if true, -1 if not
------------------------------------------------------------------------*/
int FileExists(char *sPath, char *sName) {
    struct stat st_buf;
    
    int status = stat (sPath, &st_buf);
    
    if (status != 0) {
		fprintf(stderr, "FileIsFile: unreachable file %s%s\n", sPath,sName );
        return -1;
    }

    if (S_ISREG (st_buf.st_mode)) {
		return 0;
    }

    return -1;
}

/***********************************************************************	
  CreateLock - write a LOCK-File 
  name : LOCKFILE with path
  progname : running program name 
  return : -1 at error, 0 else
***********************************************************************/ 
int CreateLock(char *name, char *progname)
{
  FILE *lock_fd;
  int retu = -1;
  pid_t pid;
  char *user;  

  printd("CreateLock -%s-%s-\n",name,progname);
      
  pid = getpid();
  user = (char *)getenv("LOGNAME");  /* get login name */

  printf("writing new LOCKFILE %s \n",name);
  if( (lock_fd = fopen(name,"w")) != NULL )
    {
      fprintf(lock_fd,"%.5d %s %s", pid,progname,user);
      fclose(lock_fd);
      retu = 0;
    }
  else fprintf(stderr,"Can't write LOCKFILE %s\n",name);
  return retu;
}

/**********************************************************************	
  CheckLock - check an existing LOCK-File 
  name  : LOCKFILE with path
  return: -1 on error, Process ID (PID) while process is running, 0 else

lockfile contains: PID executablename user
***********************************************************************/ 
int CheckLock(char *name)
{
  FILE *lock_fd;
  int retu = -1;
  FILE *proc_fd;
  char buff1[10], buff2[20], buff3[20];
  char buff[30];
  pid_t pid;
  
  printd("CheckLock -%s-\n",name);
      
  if( (lock_fd = fopen(name,"r")) != NULL )
    {
      printf("LOCKFILE %s exists\n",name);
      fscanf(lock_fd,"%08s %18s %18s",buff1,buff2,buff3);
      pid = atoi(buff1);
      if(pid > 0) {  /* print only if something exists */
      /* fprintf(stderr,"file carries: %s %s %s\n",buff1,buff2,buff3); */ /* not an error */
      printf("  file carries: %s %s %s\n",buff1,buff2,buff3);
      sprintf(buff,"/proc/%d/comm",pid);
      /* fprintf(stderr,"searching inside of /proc for: %s\n",buff); */ /* not an error */
      printf("  searching inside of /proc for: %s\n",buff);
      }
      if( (proc_fd = fopen(buff,"r")) != NULL)
	{
	  fprintf(stderr,"%s with PID:%d already exists, started by user %s\n",buff2,pid,buff3);
	  retu = pid;
	}
      else
	{
	  printf("  PID:%d does not exist, LOCKFILE can be deleted\n",pid);
	  retu = 0;
	}
    }
  else 
    {
      printd("LOCKFILE [%s] does not exists\n",name);
      retu = 0;	
    }
    
  return retu;
}

/**********************************************************************	
  DeleteLock - deletes the own lockfile
  filename  : LOCKFILE with path
  return: remove() value

lockfile contains: PID executablename user
***********************************************************************/ 
int DeleteLock( char *filename)
{
  printd("DeleteLock -%s-\n",filename);
  return remove(filename);
}

/**********************************************************************	
  RaspberryPiVersion - returns the hardware version string
  in: nothing
  return: zero or error

lockfile contains: PID executablename user
***********************************************************************/ 
int RaspberryPiVersion(void)
{
  FILE *ver_fd;
  int i_ret = 0;
  int ch;
  
  if( (ver_fd = fopen("/sys/firmware/devicetree/base/model","r")) != NULL )
    {
      printf("Raspberry Pi Version: ");
    ch = getc(ver_fd);
    while( ch != EOF)
      {
        /* display contents of file on screen */ 
        putchar(ch);  
        ch = getc(ver_fd); 
      }
      fclose(ver_fd);
      printf("\n");
    }
  else 
    {
      printd("file /sys/firmware/devicetree/base/model does not exists\n");
      i_ret = -1;	
    }
    
  return i_ret;
}

/**********************************************************************	
  LinuxVersion - returns the Linux version string
  in: nothing
  return: zero or error

lockfile contains: PID executablename user
***********************************************************************/ 
int LinuxVersion(void)
{
  FILE *ver_fd;
  int i_ret = 0;
  int ch;
  
  if( (ver_fd = fopen("/proc/version","r")) != NULL )
    {
      printf("Linux Version: ");
    ch = getc(ver_fd);
    while( ch != EOF)
      {
        /* display contents of file on screen */ 
        putchar(ch);  
        ch = getc(ver_fd); 
      }
      fclose(ver_fd);
      printf("\n");
    }
  else 
    {
      printd("file /proc/version does not exists\n");
      i_ret = -1;	
    }
    
  return i_ret;
}

