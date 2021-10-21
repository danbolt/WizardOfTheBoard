const fs = require('fs');
const buffer = require('buffer').Buffer;

const generatedDirectoryPrefix = 'generated';

const names = fs.readdirSync(generatedDirectoryPrefix);

const binaryFilesToPack = names.map((name) => { return generatedDirectoryPrefix + '/' + name; })

const binaryFileBuffers = binaryFilesToPack.map((path => { return fs.readFileSync(path, { encoding: null }) }));

const mapping = [];

for (let i = 0; i < names.length; i++) {
	mapping.push({
		offset: ((4096 * 2) * i),
		name: names[i].split('.')[0]
	})
}

const packedBuffer = Buffer.concat(binaryFileBuffers);

fs.writeFile('packedtextures.bin', packedBuffer, {}, (err) => {
	if (err) {
		console.log(err);
	} else {
		console.log('Finished writing packed textures!');
	}
})

let gperfData = `%{#include "castlookup.h"
#include "ultratypes.h"
#include "nustdfuncs.h"

%}
struct castMappingData;
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
