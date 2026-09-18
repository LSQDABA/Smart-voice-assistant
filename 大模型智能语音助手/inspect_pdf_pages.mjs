import { createRequire } from "node:module";
import fs from "node:fs";

const require = createRequire("C:/Users/li/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/node_modules/");
const { PDFDocument } = require("pdf-lib");

const input = process.argv[2];
const data = fs.readFileSync(input);
const pdf = await PDFDocument.load(data);
console.log(`pages=${pdf.getPageCount()}`);
