// Getr current environment
var env = process.env.NODE_ENV || 'development';
console.log(`Current env: ${env}`);

// Load config file
var config = require('./config.json');
var envConfig = config[env];

// Add to environment vairables
Object.keys(envConfig).forEach((key) => {
    process.env[key] = envConfig[key];
});
