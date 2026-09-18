import fs from "node:fs";
const pdfjs = await import("file:///C:/Users/li/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/node_modules/pdfjs-dist/legacy/build/pdf.mjs");

const input = process.argv[2];
const data = new Uint8Array(fs.readFileSync(input));
const doc = await pdfjs.getDocument({ data, disableFontFace: true, useSystemFonts: true }).promise;
console.log(`pages=${doc.numPages}`);
for (let pageNo = 1; pageNo <= Math.min(doc.numPages, 5); pageNo++) {
  const page = await doc.getPage(pageNo);
  const text = await page.getTextContent();
  const joined = text.items.map((item) => item.str).join("").slice(0, 120);
  console.log(`page${pageNo}=${joined}`);
}
