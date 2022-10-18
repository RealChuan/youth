import httpx

def print_response(r):
    print(r)
    print(r.status_code)  # 状态码
    print(r.encoding)  # 文本编码
    print(r.headers)
    print(r.text)
    print('-------------------------------------------------')
#    print(r.json())  

r = httpx.get('http://127.0.0.1:8000/GET')
print_response(r)

r = httpx.head('http://127.0.0.1:8000/HEAD')
print_response(r)

r = httpx.post('http://127.0.0.1:8000/POST')
print_response(r)

r = httpx.put('http://127.0.0.1:8000/test.txt', data=r'123456789qwertyu')
print_response(r)

r = httpx.get('http://127.0.0.1:8000/test.txt')
print_response(r)

r= httpx.delete('http://127.0.0.1:8000/test.txt')
print_response(r)


