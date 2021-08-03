import datetime
from ptk.utils import inspect_frame

import datetime
import numpy as np


_time_format = "%Y-%m-%d %H:%M:%S"

class Time(object):

    def __init__(self, t=None):
        if t:
            self.date = t
        else:
            self.date = datetime.datetime.now()

    def __repr__(self):
        return self.date.strftime(_time_format)

    @property
    def seconds(self) -> int:
        return self.date.total_seconds()


    def __call__(self):
        return self.date

    def __add__(self, t):
        if isinstance(t, Time):
            compare_time = t.date
        else:
            compare_time = t
        return Time(self.date + compare_time)
    
    def __sub__(self, t):
        if isinstance(t, Time):
            compare_time = t.date
        else:
            compare_time = t
        return Time(self.date - compare_time)

    def __ge__(self, t):
        if isinstance(t, Time):
            compare_time = t.date
        else:
            compare_time = t
        return self.date >= compare_time

    def __le__(self, t):
        if isinstance(t, Time):
            compare_time = t.date
        else:
            compare_time = t
        return self.date <= compare_time

    def __gt__(self, t):
        if isinstance(t, Time):
            compare_time = t.date
        else:
            compare_time = t
        return self.date > compare_time

    def __lt__(self, t):
        if isinstance(t, Time):
            compare_time = t.date
        else:
            compare_time = t
        return self.date < compare_time

    def __eq__(self, t):
        if isinstance(t, Time):
            compare_time = t.date
        else:
            compare_time = t
        return self.date == compare_time


    @classmethod
    def now(cls):
        return cls()

    @classmethod
    def decode(cls, time_str):
        if time_str is 'None':
            return None
        date_obj = datetime.datetime.strptime(time_str, _time_format)
        return cls(date_obj)

    def encode(self):
        return self.date.strftime(_time_format)


#hash to duration object
_duration_objects = {}
class _DurationInstance():


    #period in seconds
    #-1 for wait foreever
    def __init__(self, period: float, hash_str:str):
        if period < 0:
            #just set to -1
            period = -1
        self._period = period
        self._hash = hash_str
        self._time_start = datetime.datetime.now()
        self._time_delta = datetime.timedelta(seconds=period)
        self._end_time = self._time_start +  self._time_delta
        self.last_called_time = None

    @property
    def end_time(self):
        self.last_called_time = datetime.datetime.now()
        return self._end_time
    
    @property
    def time_delta(self):
        self.last_called_time = datetime.datetime.now()
        return self._time_delta

    def __bool__(self):
        compare_time = datetime.datetime.now()
        result = compare_time >= self.end_time
        if result:
            del _duration_objects[self._hash]
        return not result

    def __ge__(self, time):
        if isinstance(time, datetime.timedelta):
            return not time >= self.time_delta 

        if isinstance(time, Time):
            compare_time = time.date
        else:
            compare_time = time

        return not compare_time >= self.end_time 

    def __le__(self, time):
        if isinstance(time, datetime.timedelta):
            return not time <= self.time_delta 

        if isinstance(time, Time):
            compare_time = time.date
        else:
            compare_time = time

        return not compare_time <= self.end_time 

    def __gt__(self, time):
        if isinstance(time, datetime.timedelta):
            return not time > self.time_delta 

        if isinstance(time, Time):
            compare_time = time.date
        else:
            compare_time = time

        return not compare_time > self.end_time 

    def __lt__(self, time):
        if isinstance(time, datetime.timedelta):
            return not time < self.time_delta 

        if isinstance(time, Time):
            compare_time = time.date
        else:
            compare_time = time

        return not compare_time < self.end_time 

    #currently must be exactly equal which most likely won't happen so use with caution
    #suggestion is to add an approx duration function
    def __eq__(self, time):
        if isinstance(time, datetime.timedelta):
            return not time == self.time_delta 

        if isinstance(time, Time):
            compare_time = time.date
        else:
            compare_time = time

        return compare_time == self._end_time 

import hashlib


def Duration(period: float):
    """[Creates a duration object that can be used as a timedelta or as a simplistic way to
    keep a result true for a given period of time.
    
    while Duration(4):
        #do stuff

    will loop for 4 seconds before exiting. This is similar to
    while Time() < Duration(4):
        #do stuff

    Duration(-1) will loop forever.

    ]

    Args:
        period (float): [Time in seconds to wait]

    Returns:
        [_DurationInstance]: [The _DurationInstance object that actually does the comparison. ]
    """

    filename, line_number, _ = inspect_frame(2)
    
    #make hash of object
    str_2_hash = filename + str(line_number) + str(period)
    hash_str = hashlib.md5(str_2_hash.encode()).hexdigest()

    if hash_str in _duration_objects:
        duration_instance = _duration_objects[hash_str]
    else:
        duration_instance = _DurationInstance(period, hash_str)
        _duration_objects[hash_str] = duration_instance

    return duration_instance