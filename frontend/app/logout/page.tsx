'use client'
import Cookies from "js-cookie";


export default function logout(){
	Cookies.remove("token");
	Cookies.remove("email");

	window.location.href = "/login";

	return (
		<div>Logout</div>
	)
}