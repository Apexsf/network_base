**working in progress**

## Tips
- send message over TCP/UDP to a listening port for debugging:  `nc [ip] [port]`

## Notices
### epoll

- When client closes the tcp connection, the connected fd of server side will be always **readable**
(in this circumstance, calling ```recv``` for arbitrary times will always return 0 indicating 
EOF). 
Therefore, ```epoll_wait``` will return immediately all the time if this connected fd being registered
to the corresponding epoll fd as read event (EPOLLIN) in level trigger mode.
To avoid this, we may choose to close the connected fd once ```recv``` return 0. See [SO](https://stackoverflow.com/questions/14563134/epoll-loops-on-disconnection-of-a-client) for more details.

