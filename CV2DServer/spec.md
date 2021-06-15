## Communicating with CV2D server

**CV2D** server is only understanding **JSON** command. Each command must have a field named ***command*** with the name of the command. the name could be upper or lower case, it don't matter. For commands that require argument, an optional field named ***args*** can be added to the JSON in order to pass an array of data.  Each data passed into this array must be an object with a ***Key : Value*** form.

**Example**

```json
{"command":"AddPlayer", "args":[{"id":"02932AD8F2"}, {"map":"testMap"}]}
```

```
{"command":"GetState"}
```



## **Commands**

#### Getmap 

**Getmap** is the fundamental function of CV2D game server, it attempt to load a map (e.g a CV) then it return it as base64 encoded data. The loaded map is a tiled map formated in JSON.

| Argument |  Type  |                   description |
| -------- | :----: | ----------------------------: |
| path     | String | the map name you want to load |

**Example**

```json
{"command":"getmap","args":[{"name":"testMap"}]}
```

**Result**

```json
{"command":"getmap","args":[{"data":"eyAiY29tcH...RoIjo1Dp9"}]}
```

------

#### AddPlayer

**AddPlayer** is used to add player to an existing instance (localized by a map name). An Id for the player to create must be passed as argument thus as the map name where to put the player

| Argument          |  Type  |                                    description |
| ----------------- | :----: | ---------------------------------------------: |
| player_id         | String |                    The Id of the player insert |
| name              | String |        The map instance where to insert player |
| ***Return type*** |        |                                                |
| instance_id       | String | the instance ID of the freshly inserted player |

******

**Example**

```json
{"command":"addplayer","args":[{"player_id":"player1"},{"name":"testMap"}]}
```

**Result**

```json
{"command":"addplayer","args":[{"instance_id":"654851235487"}]}
```

**Remarks**

The instance id returned by the function is used to authenticate the socket which is connecting to the web server. When a new instance is created, CV2D game server will open a connection to the web server.  This opened connection can be used by web server to send commands to the instance. But first, the webserver must authenticate the socket by sending the command Authenticate (see command below). After authentication have succeeded , the socket can be used to freely send command to the instance, as long as this one is open.



## Architecture

![CV2D Game Server](https://i.imgur.com/cdPmbbB.png)



CV2D is dual connection based. The principle is to connect the webServer to the CV2D server and each instance openned by CV2D is a client connecting to the webServer. This client is the only way to retrieve information about the current instance. It authenticate to the web server via a token shared with the webserver
when the instance is created.

```

   ┌────────────────────────┐                        ┌────────────────────────┐
   │ CV2D                   │                        │ WebServer              │
   │               ┌──────┐ │         GetMap         │ ┌──────┐               │
   │    Main server│Server│◄├────────────────────────┼─┤Client│ Remote client │
   │               └──┬───┘ │                        │ └──────┘               │
   │                  │     │                        │                        │
   │               ┌──▼───┐ │    Authentification    │ ┌───────┐              │
   │     Instance 1│Client├─┼────────────────────────┤►│Server │              │
   │               └──────┘ │                        │ └────┬──┘              │
   │                        │                        │      │                 │
   │               ┌──────┐ │                        │ ┌────▼─────────────┐   │
   │     Instance 2│Client│ │                        │ │Socket Instance 1 │   │
   │               └──────┘ │                        │ └──────────────────┘   │
   │                        │                        │                        │
   │                        │                        │                        │
   └────────────────────────┘                        └────────────────────────┘
```



─────────────────────────────────────────────────────────────────────────────────────
When a socket between CV2D (which is a client) and WebServer, then CV2D will send periodicaly the state
of the actual instance. If this instance is empty (no player connected) for more than a predifined timeout.
Then CV2D will close the connection.

    ┌────────────────────────┐                        ┌────────────────────────┐
    │          CV2D          │                        │        WebServer       │
    │                        │                        │                        │
    │             ┌──────┐   │                        │  ┌──────────────────┐  │
    │   Instance 1│Client│   │                        │  │Socket Instance 1 │  │
    │             └──────┘   │                        │  └────────┬─────────┘  │
    │           │            │     SendState          │           │            │
    │           │ ───────────┼────────────────────────┼────────►  │            │
    │                        │                        │                        │
    │           │ │TickRate  │                        │           │            │
    │           │ ▼          │   ┼                    │           │            │
    │                        │     SendState          │                        │
    │           │ ───────────┼────────────────────────┼────────►  │            │
    │           │            │                        │           │            │
    │             │TickRate  │                        │                        │
    │           │ ▼          │                        │           │            │
    │           │            │     SendState          │           │            │
    │              ──────────┼────────────────────────┼────────►               │
    │           │            │                        │           │            │
    │           │            │                        │           │            │
    │                        │     Close              │                        │
    │           │ ───────────┼────────────────────────┼────────►  │            │
    │           │            │                        │           │            │

