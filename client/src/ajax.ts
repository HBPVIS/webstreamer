export function AJAXGetTextRequest(url: string,
    successCallback: (response: string) => void,
    failureCallback: (status: number, statusText: string) => void) {

    const request = new XMLHttpRequest();
    request.onload = (data) => {
        if (request.status === 200) {
            successCallback(request.response);
        } else {
            failureCallback(request.status, request.statusText);
        }
    };
    request.open("GET", url);
    request.send();
}