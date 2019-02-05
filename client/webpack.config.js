const path = require('path');
const HtmlWebpackPlugin = require('html-webpack-plugin');

module.exports = {
	entry: "./src/index.ts",
	output: {
    	path: path.resolve(__dirname, 'dist'),
		filename: "webstreamer.js"
	},

	mode: "development",
    
    // Enable sourcemaps for debugging webpack's output.
    devtool: "source-map",
	
	module: {
    	rules: [
	  		{
				// all files with a '.ts' or '.tsx' extension will be handled by 'ts-loader'
                test: /\.tsx?$/,
				use: 'ts-loader',
				exclude: /node_modules/
			},
            {
                test: /\.css$/,
                use: ["style-loader", "css-loader"]
            },
            {
                test: /\.less$/,
                use: ["style-loader", "css-loader", "less-loader"]
            },
            {
                test: /\.(gif|png|jpe?g|svg)$/i,
                use: ["file-loader"]
            }
		]
	},
	resolve: {
        // Add '.ts' and '.tsx' as resolvable extensions.
        extensions: [".webpack.js", ".web.js", ".ts", ".tsx", ".js"]
    },

    plugins: [
        new HtmlWebpackPlugin({
            filename: "index.html",
            template: "src/index.html",
            inject: true
        })
    ]
};
