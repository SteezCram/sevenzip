const sevenZipBin = require('7zip-bin');
const child_process = require('child_process');
const fs = require('fs');
const path = require('path');




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
module.exports.compress = function (algorithm, parameters, callback = undefined, progressCallback = undefined) 
{
    if (!parameters)
        throw 'Parameters cannot be undefined or null';
    if (parameters.dir && parameters.files)
        throw 'Cannot use dir and files property at the same time';
    if (!parameters.destination && parameters.files)
        throw 'Cannot use files property without destination property';
    if (!parameters.destination && parameters.dir)
        parameters.destination = path.dirname(parameters.dir);

    algorithm = (algorithm === '' || algorithm === null || algorithm === undefined) ? '7z' : algorithm;

    if (fs.existsSync(parameters.destination)) {
        if (fs.lstatSync(parameters.destination).isDirectory()) {
            if (parameters.dir === undefined)
                throw 'Destination is a directory';
    
            parameters.destination = path.join(parameters.destination, `${path.basename(parameters.dir)}.${algorithm}`);
        }
    }


    const dllPath = sevenZipBin.path7za;

    return new Promise((resolve, reject) => 
    {
        const sevenZipProcess = child_process.execFile(dllPath, buildCommandArgs('compress', parameters, algorithm), { shell: true, detached: false }, (error, stdout, stderr) =>
        {
            if (progressCallback) {
                progressCallback({
                    progress: 100,
                    fileProcessed: ''
                });
            }
            
            if (callback) callback(error);
            
            if (error) reject(error);
            else resolve();
        });

        let send = false;
        sevenZipProcess.stdout.on('data', (data) => {
            if (data.includes('1%'))
                send = true;
            else if (data.includes('99%'))
                send = false;

            if (progressCallback && send) {
                progressCallback(parseProgress(data));
            }
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
module.exports.extract = function (algorithm, parameters, callback = undefined, progressCallback = undefined) 
{
    if (!parameters)
        throw 'Parameters cannot be undefined or null';

    algorithm = (algorithm === '' || algorithm === null) ? '7z' : algorithm;


    const dllPath = sevenZipBin.path7za;

    return new Promise((resolve, reject) => 
    {
        const sevenZipProcess = child_process.execFile(dllPath, buildCommandArgs('extract', parameters), { shell: true, detached: false }, (error, stdout, stderr) =>
        {
            if (progressCallback) {
                progressCallback({
                    progress: 100,
                    fileProcessed: ''
                });
            }

            if (callback) callback(error);

            if (error) reject(error);
            else resolve();
        });
        
        let send = false;
        sevenZipProcess.stdout.on('data', (data) =>
        {
            if (data.includes('1%'))
                send = true;
            else if (data.includes('99%'))
                send = false;

            if (progressCallback && send) {
                progressCallback(parseProgress(data));
            }
        });
    });
}




/** Build the command line
 * 
 * @param {string} operation - operation to do
 * @param {object} parameters - parameters to use
 * @param {string} algorithm - operation to do
 * 
 * @return {string} - command line
 */
function buildCommandArgs(operation, parameters, algorithm = undefined)
{
    const arguments = [operation === 'compress' ? 'a' : 'x'];

    switch (operation) 
    {
        case 'compress':
            arguments.push(`-t${algorithm}`)
            arguments.push(`"${parameters.destination}"`);
            arguments.push(`"${parameters.dir}"`);

            switch (algorithm)
            {
                case '7z':
                case 'xz':
                    arguments.push('-m0=LZMA2');

                    if (parameters.level !== undefined)
                        arguments.push(`-mx=${parameters.level}`);
                    
                    if (parameters.password !== undefined)
                        arguments.push(`-p${parameters.password}`)
                    break;
                    
                case 'zip':
                    if (parameters.is64 !== undefined && parameters.is64)
                        arguments.push('-mm=Deflate64');
                    else
                        arguments.push('-mm=Deflate');

                    if (parameters.level !== undefined)
                        arguments.push(`-mx=${parameters.level}`);
                    break;
            }
            break;

        case 'extract':
            arguments.push(`"${parameters.archive}"`);
            arguments.push(`-o"${parameters.destination}"`);
            arguments.push('-aoa');
            break;
    }


    arguments.push('-bsp1');
    return arguments;
}

/** Parse the stdout data to a progress object
 * 
 * @param {string} data - data to parse
 * 
 * @return {object} - progress callback object
 */
function parseProgress(data)
{
    data = data.trim();
    const dataArr = data.split(' ');

    return {
        progress: parseInt(dataArr[0].slice(0, -1), 10),
        fileProcessed: dataArr.slice(3).join(' '),
    }
}