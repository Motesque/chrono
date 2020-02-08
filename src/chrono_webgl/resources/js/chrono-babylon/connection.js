export function getHostAndPort() {
    return location.hostname+(location.port ? ':'+location.port: '');;
  }

export class ChronoPersistentConnection {
    constructor(onOpen, onClose, onMessage) {
        this.socket = null;
        this.onOpen = onOpen;
        this.onClose = onClose;
        this.onMessage = onMessage;
        this._open();
    }

    _open() {
        const hostAndPort = location.hostname+(location.port ? ':'+location.port: '');
        this.socket = new WebSocket(`ws://${hostAndPort}/ws`);
        this.socket.binaryType = "arraybuffer";
        this.socket.onopen = (evt) => {   
            console.log("[open] Connection established"); 
            this.onOpen();
        };
        this.socket.onclose = (evt) => { 
            console.log(`[close] Connection closed. wasClean=${evt.wasClean}`); 
            this.onClose();
            setTimeout(() => this._open(), 1024);
        };
        this.socket.onerror = (evt) => { console.log(`[error] Connection error. error="${evt.message}"`); };
        this.socket.onmessage = (evt) => { 
            this.onMessage(evt.data);
        };
    };

}
