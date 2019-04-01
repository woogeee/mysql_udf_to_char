/*  Copyright (c) 2010, 2016, Oracle and/or its affiliates. All rights reserved.
    
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; version 2 of the
    License.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <my_global.h>
#include <mysql_version.h>
#include <mysql/plugin.h>
#include <mysql_com.h>


static struct st_mysql_daemon to_char_plugin=
        { MYSQL_DAEMON_INTERFACE_VERSION  };

    mysql_declare_plugin(to_char)
        {
                MYSQL_DAEMON_PLUGIN,
                &to_char_plugin,
                "to_char",
                " ",
                " ",
                PLUGIN_LICENSE_GPL,
                NULL,                       /* Plugin Init          */
                NULL,                       /* Plugin Deinit        */
                0x0100,                     /* Plugin version: 1.0  */
                NULL,                       /* status variables     */
                NULL,                       /* system variables     */
                NULL,                       /* config options       */
                0,                          /* flags                */
        }
mysql_declare_plugin_end;

#ifdef WIN32
#define PLUGIN_EXPORT extern "C" __declspec(dllexport)
#else
#define PLUGIN_EXPORT extern "C"
#endif

extern "C"
{
	char* to_char(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);
	my_bool to_char_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
	void to_char_deinit(UDF_INIT *initid);
//	void to_char_clear(UDF_INIT *initid, char *is_null, char *error);
//	void to_char_add(UDF_INIT* initid, UDF_ARGS *args, char *is_null, char *error);
}

typedef struct st_buffer
{
	char ori_time[100];
	char format[100];
} MY_BUFFER;

my_bool to_char_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
	MY_BUFFER * pbuffer;

	if(args->arg_count != 2)
	{
	    strcpy(message, "to_char() can only accept two arguments");
	    return 1;
	}
   //	if(args->args[0] == NULL)
//	{
//		strcpy(message, "to_char() frist argument is NULL");
//		return 1;
//	}	

//    	if(args->arg_type[0] != REAL_RESULT)
// 	{
//	    strcpy(message, "to_char() argument has to be an integer");
//	    return 1;
 //   	}
    	
	pbuffer = (MY_BUFFER*)malloc(sizeof(MY_BUFFER));
	memset(pbuffer->format, 0, 100);
	memset(pbuffer->ori_time,'0',100);
//	strcpy(pbuffer->ori_time, "aaaa");
//	memcpy(pbuffer->ori_time,args->args[0], 4);
//	strcpy(pbuffer->format, args->args[1]);

//	initid->ptr = (char*)malloc(strlen(args->args[1]) + 1);
	initid->ptr = (char*)pbuffer;
	return 0;
}

void to_char_deinit(UDF_INIT *initid)
{
	free(initid->ptr);
}

char* to_char(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error)
{
	char* substr;
//	char* ori_str = args->args[0];	

	MY_BUFFER* pbuffer = (MY_BUFFER*)initid->ptr;

//	return args->args[0];
	if(args->args[0])
	{
		strcpy(pbuffer->ori_time, args->args[0]);
		if(strlen(pbuffer->ori_time) < 11)
		{
			memset(pbuffer->ori_time+10, '0', 20);
		}
	}
	if(args->args[1])
	{
		char* stmp;
		
		strcpy(pbuffer->format, args->args[1]);
		stmp = pbuffer->format;

		for(; *stmp; ++stmp) *stmp = tolower(*stmp);
	}

//	strcpy(initid->ptr, args->args[1]);

	substr = strstr(pbuffer->format, "yyyy");	
	
//	return substr;
//	return ori_str;

	//year
	if(substr)
	{
		memcpy(substr, pbuffer->ori_time, 4); 
	}

	//month
	if(substr = strstr(pbuffer->format, "mm"))
	{
		memcpy(substr, pbuffer->ori_time+5, 2);
	}

	//day
	if(substr = strstr(pbuffer->format, "dd"))
	{
		memcpy(substr, pbuffer->ori_time+8, 2);
	}
	
	//hour	
	if(substr = strstr(pbuffer->format, "hh24"))
	{
		strcpy(substr+2,substr+4);
		memcpy(substr, pbuffer->ori_time+11, 2);
	}
	else if(substr = strstr(pbuffer->format, "hh"))
	{
		char shour[3];
		unsigned short int ihour;
		
		memcpy(shour, pbuffer->ori_time+11, 2);
		shour[2] = 0;
		ihour = atoi(shour);
		ihour = ihour%12;
		sprintf(shour, "%02d", ihour);
		memcpy(substr, shour, 2);
	}
	
	//minute
	if(substr = strstr(pbuffer->format, "mi"))
	{
		memcpy(substr, pbuffer->ori_time+14, 2);
	}
	
	//second
	if(substr = strstr(pbuffer->format, "ss"))
	{
		memcpy(substr, pbuffer->ori_time+17, 2);
	}

	//microsecond
	
	if(substr = strstr(pbuffer->format, "f"))
	{
		size_t sizeofmicrosecond = 1;
		char* stmp = substr;
		char* sori = pbuffer->ori_time+20;
		while(*stmp == 'f')
		{
		//	sizeofmicrosecond++;
			if(*sori)
			{
				*stmp++ = *sori++;
			}
			else
			{
				*stmp++ = '0';
				sori++;
			}
		}
		*stmp = 0;
//		memcpy(substr, pbuffer->ori_time+20, sizeofmicrosecond);


	}

    	return pbuffer->format;
}

