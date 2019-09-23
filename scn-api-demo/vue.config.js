const path = require('path')

const resolve = dir => {
  return path.join(__dirname, dir)
}

const BASE_URL = process.env.NODE_ENV === 'production'
  ? '/'
  : '/'

module.exports = {
  // Project deployment base
  // By default we assume your app will be deployed at the root of a domain,
  // e.g. https://www.my-app.com/
  // If your app is deployed at a sub-path, you will need to specify that
  // sub-path here. For example, if your app is deployed at
  // https://www.foobar.com/my-app/
  // then change this to '/my-app/'
  baseUrl: BASE_URL,
  // tweak internal webpack configuration.
  // see https://github.com/vuejs/vue-cli/blob/dev/docs/webpack.md
  // If you don't need to use eslint, set lintOnSave to false
  lintOnSave: false,
  chainWebpack: config => {
    config.resolve.alias
      .set('@', resolve('src')) //custom  key,value ，eq: .set('@@', resolve('src/components'))
      .set('_c', resolve('src/components'))
  },
  //  If you don't generate .map file, set productionSourceMap to false
  productionSourceMap: false,
  devServer: {
    proxy: {
      '/api':{
        target:'https://gateway.qzcloud.com',
        changeOrigin: true
      }
    }
  }
}
