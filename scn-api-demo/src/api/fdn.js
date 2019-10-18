import axios from '@/libs/api.request'

export const uploadBase64 = info => {
    return axios.request({
      url:'',// api url
      data: info,
      method: 'post',
      headers:{"x-fdn-sign" :""}// fdn token
    })
  }