const fs = require('fs');
const path = require('path');
const sevenZip = require('./sevenzip-node.node');




/**
 * Compression level enum, use it to access all the compression level
*/
module.exports.CompressionLevel = {
    NONE: 0,
    FASTEST: 1,
    FAST: 3,
    NORMAL: 5,
    MAX: 7,
    ULTRA: 9,
}

/**
 * Compress some files or a whole directory
 * 
 * @param {string} algorithm - algorithm to use
 * @param {object} parameters - parameters to use
 * 
 * @return {promise} - pending operation
*/
module.exports.compress = function (algorithm, parameters, callback) 
{
    if (algorithm === '')
        algorithm = '7z';

    if (fs.lstatSync(parameters.destination).isDirectory()) {
        if (parameters.dir === undefined)
            throw 'Destination is a directory';

        parameters.destination = path.join(parameters.destination, `${path.basename(parameters.dir)}.${algorithm}`);
    }

    if (callback !== null || callback !== undefined) {
        sevenZip.__compress(algorithm, parameters, callback);
        return;
    }

    return new Promise((resolve, reject) => {
        sevenZip.__compress(algorithm, parameters, (error) => {
            resolve(error);
        });
    });
}

/**
 * Extract an archive file
 * 
 * @param {string} algorithm - algorithm to use
 * @param {object} parameters - parameters to use
 * 
 * @return {promise} - pending operation
*/
module.exports.extract = function (algorithm, parameters, callback) 
{
    if (algorithm === '')
        algorithm = '7z';

    if (fs.lstatSync(parameters.destination).isDirectory()) {
        parameters.destination = path.join(parameters.destination, path.basename(parameters.archive, algorithm));
    }


    if (callback !== null || callback !== undefined) {
        sevenZip.__extract(algorithm, parameters, callback);
        return;
    }

    return new Promise((resolve, reject) => {
        sevenZip.__extract(algorithm, parameters, (error) => {
            resolve(error);
        });
    });
}