const express = require('express');
const addon = require('./build/Release/addon.node'); // файл с вашим node-addon-api

const app = express();
const port = 3000;


app.get('/', (req, res) => {
    res.sendFile(__dirname + '/index.html');
});

app.get('/check-user', (req, res) => {
    const username = req.query.username;
    const result = addon.checkUser(username);
    res.send(result);
});

app.listen(port, () => {
    console.log(`Server listening at http://localhost:${port}`);
});