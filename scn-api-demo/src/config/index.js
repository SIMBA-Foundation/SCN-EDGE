export default {
  /**
   * @description page title
   */
  title: 'Blockcloud',
  /**
   * @description The number of days the token is stored in the Cookie, the default is 1 day
   */
  cookieExpires: 1,
  /**
   * @description internationalization setting , default value is false
   */
  useI18n: true,
  /**
   * @description api request path
   */
  baseUrl: {
    dev: 'http://localhost:8081',
    pro: '/'
  },
  /**
   * @description 
   */
  homeName: 'home',
  /**
   * @description 
   */
  plugin: {
    'error-store': {
      showInHeader: true, 
      developmentOff: true 
    }
  }
}
