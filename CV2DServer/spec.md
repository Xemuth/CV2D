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
| **path** | String | the map name you want to load |

**Example**

```json
{"command":"getmap","args":[{"path":"testMap"}]}
```

**Result**

```json
{"command":"getmap","args":[{"data":"eyAiY29tcH...RoIjo1Dp9"}]}
```



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

