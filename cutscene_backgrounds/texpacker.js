const fs = require('fs');
const buffer = require('buffer').Buffer;

const generatedDirectoryPrefix = 'generated';

const names = fs.readdirSync(generatedDirectoryPrefix);

const binaryFilesToPack = names.map((name) => { return generatedDirectoryPrefix + '/' + name; })

const binaryFileBuffers = binaryFilesToPack.map((path => { return fs.readFileSync(path, { encoding: null }) }));

const mapping = [];

for (let i = 0; i < names.length; i++) {
	mapping.push({
		offset: ((320 * 240 * 2) * i),
		name: names[i].split('.')[0]
	})
}

const packedBuffer = Buffer.concat(binaryFileBuffers);

fs.writeFile('packedbackgrounds.bin', packedBuffer, {}, (err) => {
	if (err) {
		console.log(err);
	} else {
		console.log('Finished writing packed textures!');
	}
})

let gperfData = `%{#include "backgroundlookup.h"
#include "ultratypes.h"
#include "nustdfuncs.h"

%}
struct backgroundMappingData;
%%
`;
mapping.forEach((mappingItem) => {
	gperfData += mappingItem.name + ',' + mappingItem.offset + '\n';
})
fs.writeFile('texture-gperf-mapping', gperfData, {}, (err) => {
	if (err) {
		console.log(err);
	} else {
		console.log('Finished writing gperf info!');
	}
})
