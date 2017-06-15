#!/usr/bin/python3
# coding: utf-8 


from datetime import timedelta, date, datetime
from pytz import timezone

x1 = date.today();
print("Today's date is", x1)

x2 = datetime.now();
print("Today's datetime is", x2) 

time_delta = timedelta(weeks=1, days=2, hours=3, seconds=240, microseconds=10000)
print("Here the delta time we want to add ", time_delta)
print("Here the added result", x2 + time_delta)

x3 = datetime.now(tz=timezone("Asia/Tehran"));
print("Time in", x3.tzinfo, "is", x3)

x4 = datetime.strptime("21/11/92 16:30", "%d/%m/%y %H:%M")
print("datetime from strptime is", x4)

x5 = datetime.utcfromtimestamp(1497252490.0282006);
print("UTC datetime from timestamp is ", x5, ". Check timestamp back:", x5.timestamp())

print("You can substract two datetimes into a timedelta", x5 - x4)
