import Vue from 'vue'
import App from './App.vue'
//import VueRouter from 'vue-router';
//import Routers from './router.js';
import iView from 'iview';
import 'iview/dist/styles/iview.css';
//Vue.use(VueRouter);
import { locale, Page } from 'iview';
import lang from 'iview/dist/locale/en-US';

// configure language
locale(lang);

Vue.use(iView);

// Vue.config.lang = 'en-US';
// Vue.locale('en-US', en);

Vue.config.productionTip = false

new Vue({
  render: h => h(App),
}).$mount('#app')
