# Pub/Sub Event Bus

```
 ______         __       __________         __    
|   __ \.--.--.|  |--.  /  /     __|.--.--.|  |--.
|    __/|  |  ||  _  |,' ,'|__     ||  |  ||  _  |
|___|   |_____||_____/__/  |_______||_____||_____|
                                                  
```

Simple.  

* create a pub/sum instance
* subscribe with your event callback
* publish to the event
* ignore the results

Pub/Sub follows the _fire and forget_ rule. For example; lets say you have a data logging service that takes readings at specific intervals, say, temperature at every 5 minute mark.
So at the 5 minute mark you get the temperature and post it to the server.  The server then reports the new temperature.

But what if the server is down?  Well, in parallel to the post, a database entry is made and persisted.  When the service becomes active again, the new data is read in.  Hence, no data loss, just a delay in presentation.

<hr/>

###### 2025.02.18 | dpw
