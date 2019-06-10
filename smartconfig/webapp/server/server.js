const axios = require('axios');

require('./config/config');

// http socket
const http = require('http');
// API Server
const express       = require('express');
// API Server Plugin
const bodyParser    = require('body-parser');

const devices = new Array(10);
for (let i = 0; i < devices.length; i++) {
    devices[i] = {};
    
}
var isRealString = (str) => {
    return typeof str ===  'string' && str.trim().length > 0;
}

// New API Server
var app = express();

app.use(function(req, res, next) {
    // Allow CORS
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
    next();
  });
const port = process.env.PORT;

app.use(bodyParser.json());

app.get('/register', (req, res) => {
    console.log(req.query);
    // Extract information
    const device = {};
    device.id = req.query.id;
    device.type = req.query.type;
    device.ip = req.query.ip;
    device.connected = true;
    devices[req.query.id] = device;
    console.log("New Device");
    console.log(devices);
    res.send(req.query.id);
});

app.get('/devices', (req, res) => {
    console.log("Online Devices")
    console.log(devices);
    res.send(devices);
});

app.get('/status', (req, res) => {
    // console.log(`Devices ${req.query.id} status`);
    const id = req.query.id;
    if(devices[id] && devices[id].connected)
    {
        // console.log('online');
        res.send('1');
    }
    else
    {
        // console.log('offline');
        res.send('0');
    }
});

app.get('/switch', async (req, res) => {
    // console.log(`Devices ${req.query.id} status`);
    const id = req.query.id;
    if(devices[id] && devices[id].ip)
    {
        const url = 'http://' + devices[id].ip + '/switch'
        console.log(url);
        const response = await axios.get(url, {
            timeout: 3000
        })
        .then((response) => {
            console.log(response.data);
            if(response.data === 'ok')
            {
                console.log(`Switch ${id} success`);
                res.send('ok');
            }
            else
            {
                console.log(`Switch ${id} fail`);    
                console.log(response.data);    
                res.send('fail');
            }
        })
        .catch((error) => {
            console.log(`Switch ${id} fail`);    
            res.send('fail');
        });
    }
    else
    {
        // console.log('offline');
        res.send('error');
    }
});

app.get('/temp',async (req, res) => {
    // console.log(`Devices ${req.query.id} temp`);
    const id = req.query.id;
    if(devices[id] && devices[id].ip)
    {
        const url = 'http://' + devices[id].ip + '/temp'
        console.log(url);
        const response = await axios.get(url, {
            timeout: 3000
        })
        .then((response) => {
            // console.log(response.data);
            if(response.data)
            {
                console.log('Temp');
                console.log(response.data);
                res.send(String(response.data));
            }
            else
            {
                console.log(`Get temp ${id} error`);    
                console.log(response.data);    
                res.send('0');
            }
        })
        .catch((error) => {
            console.log(`Get temp ${id} fail`);    
            // console.log(error);
            res.send('0');
        });
    }
    else
    {
        console.log('get temp offline');
        res.send('0');
    }
});

app.get('/humid',async (req, res) => {
    // console.log(`Devices ${req.query.id} temp`);
    const id = req.query.id;
    if(devices[id] && devices[id].ip)
    {
        const url = 'http://' + devices[id].ip + '/humid'
        console.log(url);
        const response = axios.get(url, {
            timeout: 3000
        })
        .then((response) => {
            // console.log(response.data);
            if(response.data)
            {
                console.log('Humid');
                console.log(response.data);
                res.send(String(response.data));
            }
            else
            {
                console.log(`Get humid ${id} error`);    
                console.log(response.data);    
                res.send('0');
            }
        })
        .catch((error) => {
            console.log(`Get humid ${id} fail`);    
            console.log(error);
            // res.send('0');
        });
    }
    else
    {
        console.log('get humid offline');
        res.send('0');
    }
});

// socket
const server = http.createServer(app);
server.listen(port, () => {
    console.log(`Server started at ${port}`);
});

async function checkStatus() {
    for (let i = 0; i < devices.length; i++) 
    {
        if(devices[i] && devices[i].ip)
        {
            const url = 'http://' + devices[i].ip + '/status'
            console.log(url);
            const response = await axios.get(url, {
                timeout: 2000
            })
            .then((response) => {
                // console.log(response.data);
                if(response.data === 'ok')
                {
                    devices[i].connected = true;
                    // console.log(`Device ${i} online`);
                }
                else
                {
                    // devices[i] = {};
                    devices[i].connected = false;
                    // console.log(`Device ${i} offline`);    
                }
            })
            .catch((error) => {
                // console.error(error);
                // devices[i] = {};
                devices[i].connected = false;
                console.log(`Device ${i} offline`);
            });
        }
    }
}

app.get('/display',async (req, res) => {
    // console.log(`Devices ${req.query.id} temp`);
    const id = req.query.id;
    const data = req.query.data;
    if(devices[id] && devices[id].ip)
    {
        const url = 'http://' + devices[id].ip + '/display?data=' + data;
        console.log(url);
        const response = axios.get(url, {
            timeout: 3000
        })
        .then((response) => {
            // console.log(response.data);
            if(response.data)
            {
                res.send(String(response.data));
            }
            else
            {
                res.send('error');
            }
        })
        .catch((error) => {
            console.log(error);
            // res.send('0');
        });
    }
    else
    {
        res.send('error');
    }
});

setInterval(checkStatus, 3000);

module.exports = {
    app
};
