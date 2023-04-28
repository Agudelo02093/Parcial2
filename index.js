import express, {
    request,
    response
} from "express";
import {
    Server
} from "socket.io";
import cors from "cors";

//Serial Port Configuration

import {
    SerialPort,
    ReadlineParser
} from "serialport"
const protocolConfiguration = {
    path: 'COM3',
    baudRate: 9600
}
const serialPort = new SerialPort(protocolConfiguration);
const parser = serialPort.pipe(new ReadlineParser());



const PORT = 8080
const expressApp = express()
const httpServer = expressApp.listen(PORT, () => {
    console.table({
        'Game': `http://localhost:${PORT}/game`,
    })

})
const io = new Server(httpServer, {
    path: '/real-time'
})

expressApp.use('/game', express.static('public-game'))
expressApp.use(express.json())

io.on('connection', (socket) => {
    console.log('Connected!', socket.id)
    //
})

let currentScore = 0;

expressApp.get('/final-score', (request, response) => {
    response.send({
        content: currentScore
    });
})

/*___________________________________________

1) Create an endpoint to POST player's current score and print it on console
It should send a messago to ARDUINO to turn on and off the lights when the player scores a point
_____________________________________________ */

expressApp.post('/score', (request, response) => {
    const { score } = request.body;
    console.log(`Player's score is: ${score}`);
  
    // Send message to Arduino to turn on/off lights
    serialPort.write(score > 0 ? 'on' : 'off', (err) => {
      if (err) {
        console.error('Error writing to Arduino:', err);
      }
    });
  
    response.sendStatus(200);
  });


/*___________________________________________

2) Create an endpoint to POST that the game is over and turn on all the lights.
_____________________________________________ */

expressApp.post('/game-over', (request, response) => {
    // Send message to Arduino to turn on all lights
    serialPort.write('all-on', (err) => {
      if (err) {
        console.error('Error writing to Arduino:', err);
      }
    });
  
    response.sendStatus(200);
  });


let arduinoMessage = {
    actuatorValue: 0,
    btnAValue: 0,
    btnBValue: 0
}

parser.on('data', (data) => {
    console.log(data);
    let dataArray = data.split(' ')
    arduinoMessage.actuatorValue = parseInt(dataArray[0])
    arduinoMessage.btnAValue = parseInt(dataArray[1])
    arduinoMessage.btnBValue = parseInt(dataArray[2])

    /*___________________________________________

3) Use the socket.io instance to send the message from the ARDUINO to the client in the browser

_____________________________________________ */

// PUT IT HERE
// Emit event to send arduinoMessage to client
io.emit('arduino-message', arduinoMessage);

});