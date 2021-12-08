const fs = require('fs')
const sharedStructs = require('shared-structs')
const strings = require('shared-structs/string')
const buffer = require('buffer').Buffer;

const structs = sharedStructs(`
	struct dialogue {
		uint8_t speaker[16];
		uint8_t text[256];
		uint32_t nextAddress;
		uint8_t flags[32];
	}
`, {
	alignment: {
		dialogue: 4
	}
})

const lengthOfAStructInBytes = structs.dialogue().rawArrayBuffer.byteLength;

// const someStruct = structs.dialogue()
// console.log(someStruct);

const processedConvoFiles = fs.readdirSync('processed')

const processedConvos = processedConvoFiles.map((filename) => {
	return {
		key: filename.split('.')[0],
		data: JSON.parse(fs.readFileSync('processed/' + filename, { encoding: 'utf8' })),
		offset: 0
	}
})

// compute the appropriate byte offsets for each of the convos
let offsetDistance = 0
processedConvos.forEach((convo) => {
	convo.offset = offsetDistance;
	offsetDistance += convo.data.length * lengthOfAStructInBytes;
})

// Using said offsets, fill in the "next addresses" for each of the convos
processedConvos.forEach(convo => {
	for (let i = 0; i < convo.data.length; i++) {
		// If we're at the end of the conversation, ensure the next address is null
		if (i === (convo.data.length - 1)) {
			convo.data[i].nextAddress = 0;
			continue;
		}

		// If there are more bits of dialogue to show, put the address to the next offset
		convo.data[i].nextAddress = convo.offset + ((i + 1) * lengthOfAStructInBytes);
	}
})

// Process the offset information for gperf
const offsetInformation = processedConvos.map((convo) => { return { key: convo.key, offset: convo.offset } });
// console.log(offsetInformation);

// Extract the raw items in the order they are;
const rawDialogueItems = processedConvos.map((convo) => { return convo.data }).flat();
// console.log(rawDialogueItems);

// copied from:
// https://stackoverflow.com/questions/38314401/converting-number-to-big-endian-on-node-js
function ReverseEndian(x) {
    buf = Buffer.allocUnsafe(4)
    buf.writeUIntLE(x, 0, 4)
    return buf.readUIntBE(0, 4)
}

const FLAG_SHOW_BG_1 = 'ik1';
const FLAG_SHOW_BG_2 = 'ik2';
const FLAG_SHOW_BG_3 = 'ik3';

const STRUCT_FLAG_SHOW_BG_1 = 1;
const STRUCT_FLAG_SHOW_BG_2 = 2;
const STRUCT_FLAG_SHOW_BG_3 = 3;

const STRUCT_FLAG_PLAY_SOUND = 'sound'

const FLAG_INDEX_BG_CHANGE = 0

const FLAG_INDEX_PLAY_SOUND = 1
const FLAG_INDEX_SOUND_ID = 2

const FLAG_INDEX_BLOOD = 3
const STRUCT_FLAG_BLOOD = 'blood'

const structDialogueItems = rawDialogueItems.map((item) => {
	const struct = structs.dialogue();

	const convertedBuffer = Buffer.from(item.text, 'latin1');
	struct.text.fill(convertedBuffer, 0, item.text.length)

	strings.encode(item.speaker, struct.speaker);
	// strings.encode(item.text, struct.text); // TODO: does this support latin characters for old ISO?
	struct.nextAddress = ReverseEndian(item.nextAddress);

	if (item.flags.indexOf(FLAG_SHOW_BG_1) > -1) {
		struct.flags[FLAG_INDEX_BG_CHANGE] = STRUCT_FLAG_SHOW_BG_1;
	} else if (item.flags.indexOf(FLAG_SHOW_BG_2) > -1) {
		struct.flags[FLAG_INDEX_BG_CHANGE] = STRUCT_FLAG_SHOW_BG_2;
	}else if (item.flags.indexOf(FLAG_SHOW_BG_3) > -1) {
		struct.flags[FLAG_INDEX_BG_CHANGE] = STRUCT_FLAG_SHOW_BG_3;
	}

	item.flags.forEach((flag) => {
		if (flag.startsWith(STRUCT_FLAG_PLAY_SOUND) && flag.length > STRUCT_FLAG_PLAY_SOUND.length) {
			const soundIndexToPlayString = (flag.substr(STRUCT_FLAG_PLAY_SOUND.length));
			const soundIndex = parseInt(soundIndexToPlayString) % 256;

			struct.flags[FLAG_INDEX_PLAY_SOUND] = 1;
			struct.flags[FLAG_INDEX_SOUND_ID] = soundIndex;
		}
	})

	if (item.flags.indexOf(STRUCT_FLAG_BLOOD) > -1) {
		struct.flags[FLAG_INDEX_BLOOD] = 1;
	}

	return struct;
})
const dialogueBuffers = structDialogueItems.map((struct) => { return struct.rawBuffer });

const concatenatedDialogueBuffers = buffer.concat(dialogueBuffers);
fs.writeFile('dialogueBuffers.bin', concatenatedDialogueBuffers, {}, (err) => {
	if (err) {
		console.log(err);
	} else {
		console.log('Finished writing ' + dialogueBuffers.length + ' dialogue buffers!');
	}
})


let gperfData = `%{#include "dialoguelookup.h"
#include "ultratypes.h"
#include "nustdfuncs.h"

%}
struct dialogueMappingData;
%%
`;
offsetInformation.forEach((mappingItem) => {
	gperfData += mappingItem.key + ',' + mappingItem.offset + '\n';
})
fs.writeFile('map-gperf-mapping', gperfData, {}, (err) => {
	if (err) {
		console.log(err);
	} else {
		console.log('Finished writing gperf info for maps!');
	}
})
