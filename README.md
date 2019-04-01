## mysql udf to_char()

#This is the mysql UDF(User Defined Function).

#You can use the funtion like it on Oracle database.

#ex) 
 - select to_char(CURRENT_TIMESTAMP, 'yyyy-mm-dd hh24:mi:ss');
   ==> 2019-04-01 15:41:27
 - select to_char(NOW(6), 'yyyy-mm-dd hh24:mi:ss.ffffff');
   ==> 2019-04-01 15:41:27.053506
