export class StatGraph {
    readonly dom: HTMLElement;

    private medianText: HTMLElement;
    private minText: HTMLElement;
    private maxText: HTMLElement;
    private context: CanvasRenderingContext2D;

    private width = 100;
    private height = 50;
    private values: number[] = [];
    private color: string;

    public constructor(name: string, width: number = 100, color: string = "white") {
        this.color = color;
        this.width = width;

        this.dom = document.createElement("div");
        this.dom.style.display = "inline-block";
        this.dom.style.backgroundColor = "black";
        this.dom.style.opacity = "0.5";
        this.dom.style.color = color;
        this.dom.style.margin = "1em";

        let title = document.createElement("div");
        title.textContent = name;

        this.medianText = document.createElement("div");
        this.medianText.textContent = "Median: ";

        let container = document.createElement("div");
        container.style.padding = "0";
        container.style.margin = "0";

        let canvasContainer = document.createElement("div");
        canvasContainer.style.display = "table-cell";
        canvasContainer.style.verticalAlign = "top";

        let canvas = document.createElement("canvas");
        canvas.style.padding = "0";
        canvas.style.margin = "0";
        canvas.width = this.width;
        canvas.height = this.height;
        this.context = canvas.getContext("2d");

        canvasContainer.appendChild(canvas);


        let minMaxContainer = document.createElement("div");
        minMaxContainer.style.display = "table-cell";
        minMaxContainer.style.verticalAlign = "top";
        minMaxContainer.style.position = "relative";

        this.minText = document.createElement("div");
        this.minText.style.fontSize = "small";
        this.minText.style.padding = "0";
        this.minText.style.margin = "0";
        this.minText.style.position = "absolute";
        this.minText.style.bottom = "0";
        this.maxText = document.createElement("div");
        this.maxText.style.fontSize = "small";
        this.maxText.style.padding = "0";
        this.maxText.style.margin = "0";

        minMaxContainer.appendChild(this.maxText);
        minMaxContainer.appendChild(this.minText);

        container.appendChild(minMaxContainer);
        container.appendChild(canvasContainer);

        this.dom.appendChild(title);
        this.dom.appendChild(this.medianText);
        this.dom.appendChild(container);
    }

    public push(value: number) {
        this.values.push(value);
        if (this.values.length > this.width) {
            this.values.splice(0, 1);
        }
        this.draw();
    }

    public clear() {
        this.values.length = 0;
    }

    private draw() {
        this.context.clearRect(0, 0, this.width, this.height);

        this.context.fillStyle = this.color;
        this.context.shadowColor = "rgba(0, 0, 0, 0)";
        this.context.shadowBlur = 0;
        this.context.shadowOffsetX = 0;
        this.context.shadowOffsetY = 0;

        const xOffset = this.width - this.values.length;
        const max = Math.ceil(Math.max.apply(null, this.values));

        this.maxText.textContent = max.toString();
        this.minText.textContent = "0";

        const clone = this.values.slice(0);
        clone.sort( function(a,b) {return a - b;} );
        const half = Math.floor(clone.length/2);
        const median = clone.length % 2 ? clone[half] :
                                          (clone[half-1] + clone[half]) / 2.0;
        let sum = 0;
        for (let i = 0; i < this.values.length; ++i) {
            sum += this.values[i];
        }
        const mean = sum / this.values.length;
        let sq = 0;
        for (let i = 0; i < this.values.length; ++i) {
            const diff = this.values[i] - mean;
            sq += diff * diff;
        }
        const sd = Math.sqrt(sq / this.values.length);
        this.medianText.innerHTML = "Median: " + median.toFixed(1) + "<br>Mean: " + mean.toFixed(1) + " &plusmn; " + sd.toFixed(1);

        this.context.beginPath();
        this.context.moveTo(xOffset, this.height);
        for (let i = 0; i < this.values.length; ++i) {
            this.context.lineTo(xOffset + i, this.height - this.height / max * this.values[i]);
        }
        this.context.lineTo(this.width, this.height);
        this.context.fill();
    }
}