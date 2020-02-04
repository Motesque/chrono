import asyncio
import aiohttp
from aiohttp import WSCloseCode
from aiohttp import web
import enum
import chrono_webgl

loop = None
all_clients = []
shutdown_event = asyncio.Event()
send_queue = asyncio.Queue()

class EventType(enum.IntEnum):
    Http_Server_Ready = 0
    Client_WS_Message = 1
    Client_HTTP_Message = 2

async def send_ws_data():  
    while True:
        data = await send_queue.get()
        for conn in all_clients: 
            try:
                await conn.send_bytes(data)
            except Exception as err:
                pass

async def websocket_handler(request):
    ws = web.WebSocketResponse()
    await ws.prepare(request)
    try:
        all_clients.append(ws)
        async for msg in ws:
            chrono_webgl.post_event(EventType.Client_WS_Message, msg.data)
    except Exception as err:
        print(err)
    all_clients.remove(ws)
    print('websocket connection closed')
    return ws

async def on_shutdown(app):
    for conn in all_clients:
        await conn.close(code=WSCloseCode.GOING_AWAY, message='Server shutdown')


def queue_ws_data(data):
    def _do(): 
        send_queue.put_nowait(data)
    loop.call_soon_threadsafe(_do)

def notify_shutdown():
    def _do(): 
        shutdown_event.set()
    loop.call_soon_threadsafe(_do)
   

def main(port, web_root):
    global loop 
    loop = asyncio.get_event_loop()
    async def run():
        app = web.Application()
        app.add_routes([web.get('/ws', websocket_handler),
                        web.static('/', web_root, show_index=True)])
        app_runner = aiohttp.web.AppRunner(app)
        app.on_shutdown.append(on_shutdown)
        await app_runner.setup()
        site = web.TCPSite(app_runner, 'localhost', port)
        await site.start()
        ws_send_tsk = asyncio.create_task(send_ws_data())
        print(f'Serving on localhost:{port}, web_root="{web_root}"')
        chrono_webgl.post_event(EventType.Http_Server_Ready, None)
        await shutdown_event.wait()
        ws_send_tsk.cancel()
        await app_runner.cleanup()
    asyncio.get_event_loop().run_until_complete(run())

if __name__ == "__main__":
    main(8888,"/Users/tobiaslang/dev/chrono/src/chrono_webgl/resources")