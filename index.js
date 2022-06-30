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
module.exports.compress = async function (algorithm, parameters, callback, progressCallback) 
{
    if (!parameters)
        throw 'Parameters cannot be undefined or null';
    if (parameters.dir && parameters.files)
        throw 'Cannot use dir and files property at the same time';

    algorithm = (algorithm === '' || algorithm === null || algorithm === undefined) ? '7z' : algorithm;

    try {
        await fs.promises.access(parameters.destination);

        if (await fs.promises.lstat(parameters.destination).isDirectory()) {
            if (parameters.dir === undefined)
                throw 'Destination is a directory';
    
            parameters.destination = path.join(parameters.destination, `${path.basename(parameters.dir)}.${algorithm}`);
        }
    }
    catch {
        // No error, the destination need to be created
    }


    switch (process.platform)
    {
        case 'win32':
            // Platform specific module to load
            const preGyp = require('@mapbox/node-pre-gyp');
            const sevenZip = require(preGyp.find(path.resolve(path.join(__dirname, './package.json'))));




            parameters.dll = (parameters.dll === undefined || parameters.dll.trim() === '' || parameters.dll === null) ? path.join(__dirname, 'os', platformToOSName(), process.arch, '7z.dll') : parameters.dll;

            if (callback !== null && callback !== undefined) {
                sevenZip.__compress(algorithm, parameters, callback, progressCallback === undefined || progressCallback === null ? () => {} : progressCallback);
                return;
            }
        
            return new Promise((resolve, reject) => {
                sevenZip.__compress(algorithm, parameters, (error) => {
                    resolve(error);
                }, progressCallback === undefined || progressCallback === null ? () => {} : progressCallback);
            });

        default:
            // Platform specific module to load
            const child_process = require('child_process');




            parameters.dll = (parameters.dll === undefined || parameters.dll.trim() === '' || parameters.dll === null) ? path.join(__dirname, 'os', platformToOSName(), process.arch, '7zz') : parameters.dll;

            if (callback !== null && callback !== undefined) 
            {
                const sevenZipProcess = child_process.execFile(parameters.dll, buildCommandArgs('compress', parameters, algorithm), {shell: true, detached: false}, (error, stdout, stderr) => {
                    callback(error);
                });

                let send = false;
                sevenZipProcess.stdout.on('data', (data) => {
                    if (data.includes('1%')) {
                        send = true;
                    }
                    else if (data.includes('99%')) {
                        send = false;
                    }

                    if (send) {
                        progressCallback(parseProgress(data));
                    }
                });
                return;
            }
            
            return new Promise((resolve, reject) => 
            {
                const sevenZipProcess = child_process.execFile(parameters.dll, buildCommandArgs('compress', parameters, algorithm), {shell: true, detached: false}, (error, stdout, stderr) => {
                    resolve(error);
                });

                let send = false;
                sevenZipProcess.stdout.on('data', (data) => {
                    if (data.includes('1%'))
                        send = true;
                    else if (data.includes('99%'))
                        send = false;

                    if (send && progressCallback !== undefined && progressCallback !== null) {
                        progressCallback(parseProgress(data));
                    }
                });
            });

    }
}

/**
 * Extract an archive file
 * 
 * @param {string} algorithm - algorithm to use
 * @param {object} parameters - parameters to use
 * 
 * @return {promise} - pending operation
*/
module.exports.extract = function (algorithm, parameters, callback, progressCallback) 
{
    if (!parameters)
        throw 'Parameters cannot be undefined or null';

    algorithm = (algorithm === '' || algorithm === null) ? '7z' : algorithm;

    switch (process.platform)
    {
        case 'win32':
            // Platform specific module to load
            const preGyp = require('@mapbox/node-pre-gyp');
            const sevenZip = require(preGyp.find(path.resolve(path.join(__dirname, './package.json'))));



            
            parameters.dll = (parameters.dll === undefined || parameters.dll.trim() === '' || parameters.dll === null) ? path.join(__dirname, 'os', platformToOSName(), process.arch, '7z.dll') : parameters.dll;

            if (callback !== null && callback !== undefined) {
                sevenZip.__extract(algorithm, parameters, callback, progressCallback === undefined || progressCallback === null ? () => {} : progressCallback);
                return;
            }
        
            return new Promise((resolve, reject) => {
                sevenZip.__extract(algorithm, parameters, (error) => {
                    resolve(error);
                }, progressCallback === undefined || progressCallback === null ? () => {} : progressCallback);
            });

        default:
            // Platform specific module to load
            const child_process = require('child_process');




            parameters.dll = (parameters.dll === undefined || parameters.dll.trim() === '' || parameters.dll === null) ? path.join(__dirname, 'os', platformToOSName(), process.arch, '7zz') : parameters.dll;
            
            if (callback !== null && callback !== undefined) 
            {
                const sevenZipProcess = child_process.execFile(parameters.dll, buildCommandArgs('extract', parameters), {shell: true, detached: false}, (error, stdout, stderr) => {
                    callback(error);
                });

                let send = false;
                sevenZipProcess.stdout.on('data', (data) => {
                    if (data.includes('1%')) {
                        send = true;
                    }
                    else if (data.includes('99%')) {
                        send = false;
                    }

                    if (send && progressCallback !== undefined && progressCallback !== null) {
                        progressCallback(parseProgress(data));
                    }
                });
                return;
            }
            
            return new Promise((resolve, reject) => 
            {
                const sevenZipProcess = child_process.execFile(parameters.dll, buildCommandArgs('extract', parameters), {shell: true, detached: false}, (error, stdout, stderr) => {
                    resolve(error);
                });
                
                let send = false;
                sevenZipProcess.stdout.on('data', (data) => {
                    if (data.includes('1%'))
                        send = true;
                    else if (data.includes('99%'))
                        send = false;

                    if (send) {
                        progressCallback(parseProgress(data));
                    }
                });
            });
    }
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

            if (algorithm === '7z')
            {
                arguments.push('-m0=LZMA2');

                if (parameters.level !== undefined)
                    arguments.push(`-mx${parameters.level}`);
                
                if (parameters.password !== undefined)
                    arguments.push(`-p${parameters.password}`)
            }

            if (algorithm === 'zip')
            {
                if (parameters.is64 !== undefined && parameters.is64)
                    arguments.push('-mm=Deflate64');
                else
                    arguments.push('-mm=Deflate');

                if (parameters.level !== undefined)
                    arguments.push(`-mx${parameters.level}`);
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
        progress: parseInt(dataArr[dataArr.length - 4].slice(0, -1), 10),
        fileProcessed: dataArr[dataArr.length - 1],
    }
}

/** Convert the platform name to the OS name
 * 
 * @return {string} - OS name
 */
function platformToOSName() 
{
    switch (process.platform) 
    {
        case 'win32':
            return 'win';

        case 'darwin':
            return 'mac';

        case 'linux':
            return 'linux';
    }

    return 'none';
}