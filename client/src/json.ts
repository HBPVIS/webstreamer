export function JSONGetValue(json: any, path: string, def?: any): any {
    if (path.length === 0) {
        return json || def;
    } else if (typeof json !== "object") {
        return def;
    } else {
        const index = path.indexOf('.');
        if (index < 0) {
            return json[path] || def;
        } else {
            const childName = path.substr(0, index);
            const remainingPath = path.substr(index + 1);
            return JSONGetValue(json[childName], remainingPath, def);
        }
    }
}