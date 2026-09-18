import { createRequire } from "node:module";
import fs from "node:fs";
import path from "node:path";

const require = createRequire("C:/Users/li/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/node_modules/");
const { createCanvas } = require("@napi-rs/canvas");
const pdfjs = await import("file:///C:/Users/li/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/node_modules/pdfjs-dist/legacy/build/pdf.mjs");

const input = process.argv[2];
const outputDir = process.argv[3];

if (!input || !outputDir) {
  console.error("Usage: node render_pdf_pages.mjs input.pdf output_dir");
  process.exit(2);
}

fs.mkdirSync(outputDir, { recursive: true });

const data = new Uint8Array(fs.readFileSync(input));
const loadingTask = pdfjs.getDocument({
  data,
  disableFontFace: true,
  useSystemFonts: true,
});
const doc = await loadingTask.promise;

for (let pageNo = 1; pageNo <= doc.numPages; pageNo++) {
  const page = await doc.getPage(pageNo);
  const viewport = page.getViewport({ scale: 1.5 });
  const canvas = createCanvas(Math.ceil(viewport.width), Math.ceil(viewport.height));
  const context = canvas.getContext("2d");
  await page.render({ canvasContext: context, viewport }).promise;
  const out = path.join(outputDir, `page-${String(pageNo).padStart(2, "0")}.png`);
  fs.writeFileSync(out, canvas.toBuffer("image/png"));
  console.log(out);
}
