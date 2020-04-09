const router = new VueRouter({
    routes: [
        {
            path: 'face-detection',
            name: 'Face detection',
            meta: {
              icon: 'ios-document',
              title: 'Face detection'
            },
            component: () => import('@/view/face-detection/face-detection.vue')
          }
    ]
  })