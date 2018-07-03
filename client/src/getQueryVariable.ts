let $_GET = {};
if(document.location.toString().indexOf("?") !== -1) {
    let query = document.location
                   .toString()
                   // get the query string
                   .replace(/^.*?\?/, "")
                   // and remove any existing hash string (thanks, @vrijdenker)
                   .replace(/#.*$/, "")
                   .split("&");

    for(let i=0, l=query.length; i<l; i++) {
       let aux = decodeURIComponent(query[i]).split("=");
       $_GET[aux[0]] = aux[1];
    }
}

export function getQueryVariable(name: string): string {
    return $_GET[name];
}