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
![CV2D Game Server](https://i.imgur.com/cdPmbbB.png)


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

A complete liste of possible command between the Remote connection (aka the Main server <-- Webserver client) is disponible
bellow. A complete list of possible command between instance socket is disponible below.

    ┌────────────────────────┬────────────────────────────┬────────────────────┐
    │        Command         │        Purpose             │       Json         │
    ├────────────────────────┼────────────────────────────┼────────────────────┤
    │     GetMap             │                            │                    │
    ├────────────────────────┼────────────────────────────┼────────────────────┤
    │                        │                            │                    │
    ├────────────────────────┼────────────────────────────┼────────────────────┤
    │                        │                            │                    │
    ├────────────────────────┼────────────────────────────┼────────────────────┤
    │                        │                            │                    │
    ├────────────────────────┼────────────────────────────┼────────────────────┤
    │                        │                            │                    │
    └────────────────────────┴────────────────────────────┴────────────────────┘