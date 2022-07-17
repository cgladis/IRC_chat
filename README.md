# Internet Relay Chat

This project is about creating your own IRC server.

**Internet Relay Chat** or **IRC** is a text-based communication protocol on the Internet.
It offers real-time messaging that can be either public or private. Users can exchange
direct messages and join group channels.

Code was written with the C++ 98 standard.
By the task any external library and Boost libraries are forbidden.

### Requirements

- The server must be capable of handling multiple clients at the same time and never
hang.
- Forking is not allowed.
- Only 1 select() can be used for handling all these operations (read,
  write, but also listen, and so forth).
- You must be able to authenticate, set a nickname, a username, join a channel,
  send and receive private messages using your reference client.
- All the messages sent from one client to a channel have to be forwarded to
  every other client that joined the channel.
- You must have operators and regular users.
- Then, you have to implement the commands that are specific to operators.

### Bonus part
Here are the extra features you can add to your IRC server so it looks even more like and
actual IRC server:
- Handle file transfer.
- A bot.


## How to run program?

Clone repository and then go into the created directory and run the following command:

```shell
make test
```

You can use an actual IRC client to connect to server and test it.
For example [LimeChat](https://apps.apple.com/kz/app/limechat/id414030210?mt=12)
