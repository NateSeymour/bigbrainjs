/*
 * Hashing algorithm from https://gist.github.com/jlevy/c246006675becc446360a798e2b2d781
 */
const simpleHash = str => {
    let hash = 0;
    for (let i = 0; i < str.length; i++) {
        const char = str.charCodeAt(i);
        hash = (hash << 5) - hash + char;
    }

    return (hash >>> 0).toString(36).padStart(7, '0');
};

const cyrb64 = (str, seed = 0) => {
    let h1 = 0xdeadbeef ^ seed, h2 = 0x41c6ce57 ^ seed;
    for (let i = 0, ch; i < str.length; i++) {
        ch = str.charCodeAt(i);
        h1 = Math.imul(h1 ^ ch, 2654435761);
        h2 = Math.imul(h2 ^ ch, 1597334677);
    }
    h1 = Math.imul(h1 ^ (h1 >>> 16), 2246822507);
    h1 ^= Math.imul(h2 ^ (h2 >>> 13), 3266489909);
    h2 = Math.imul(h2 ^ (h2 >>> 16), 2246822507);
    h2 ^= Math.imul(h1 ^ (h1 >>> 13), 3266489909);

    return [h2 >>> 0, h1 >>> 0];
};

const cyrb64Hash = (str, seed = 0) => {
    const [h2, h1] = cyrb64(str, seed);
    return h2.toString(36).padStart(7, '0') + h1.toString(36).padStart(7, '0');
};