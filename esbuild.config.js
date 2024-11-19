const esbuild = require('esbuild');

esbuild.build({
  entryPoints: ['src/index.tsx'],
  bundle: true,
  outfile: 'dist/bundle.js',
  format: 'esm',
  plugins: [{
    name: 'react',
    setup(build) {
      build.onLoad({ filter: /\.[jt]sx$/ }, async (args) => {
        const { transformAsync } = require('@babel/core');
        const solidPreset = require('babel-preset-react');
        
        const source = await require('fs').promises.readFile(args.path, 'utf8');
        const transformed = await transformAsync(source, {
          presets: [[solidPreset, { typescript: true }]],
          filename: args.path,
        });
        
        return {
          contents: transformed.code,
          loader: 'js',
        };
      });
    },
  }],
});