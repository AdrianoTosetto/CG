﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class LageanoRun : MonoBehaviour {

	private Animator anim;
	private Rigidbody rb;
	private SphereCollider cl;
	public float speed;
	public float moveVertical;
	public float moveHorizontal;
	public bool canJump;
	public int runnin;
	public float speedy;
	private int health = 105;
	public int currHealth;
	public Slider healthBar;
	public bool hasJumped = false;


	// Use this for initialization
	void Start () {
		anim = gameObject.GetComponentInChildren<Animator>();
		rb = GetComponent<Rigidbody> ();
		cl = GetComponent<SphereCollider> ();
		speedy = rb.velocity.y;
		currHealth = health;
		healthBar.value = health;
	}
		

	// Update is called once per frame
	void Update() {
			
		//slider.dHP();
		Vector3 v3 = new Vector3(0.0f, Input.GetAxis("Horizontal"), 0.0f);
		transform.Rotate(v3 * speed * Time.deltaTime);
		if (rb.velocity.y < -0.5e-4) {
			anim.SetInteger ("landing", 0);
			anim.SetInteger ("falling", 1);
		} else if (rb.velocity.y > 0.5e-4) {
			anim.SetInteger ("falling", 0);
			anim.SetInteger ("landing", 0);
		} else {
			anim.SetInteger ("falling", 0);
			anim.SetInteger ("landing", 1);
			//hasJumped = false;
		}
		moveHorizontal = Input.GetAxis ("Horizontal");
		moveVertical = Input.GetAxis ("Vertical");
		if (moveVertical > 0) {
			anim.SetInteger ("running", 1);
			anim.SetInteger ("turning", 0);
			anim.SetInteger ("backing", 0);
		} else if (moveVertical < 0) {
			anim.SetInteger ("running", 0);
			anim.SetInteger ("turning", 0);
			anim.SetInteger ("backing", 1);
		} else {
			anim.SetInteger ("running", 0);
			anim.SetInteger ("turning", 0);
			anim.SetInteger ("backing", 0);
			runnin = 0;
		}
		if (moveHorizontal != 0) {
			anim.SetInteger("turning", 1);
		} else {
			anim.SetInteger("turning", 0);
		}
		/*if(Input.GetKeyUp("space") && (runnin == 1)){
			canJump = true;
		}*/
		if (!((anim.GetInteger ("falling") == 1 || anim.GetInteger ("landing") == 1 || anim.GetInteger ("landing") == 1) && hasJumped)) {
			hasJumped = true;
		} else {
			hasJumped = false;
		}
		if (speedy == 0)
			hasJumped = false;
	}
	void LateUpdate () {
		speedy = rb.velocity.y;
		if (rb.velocity.y > 0 && rb.velocity.y < 1) {
			anim.SetInteger("jumping", 0);
			//hasJumped = false;
		}
		if (Input.GetKeyDown(KeyCode.Space) && rb.velocity.y > -0.5e-4 && rb.velocity.y < 0.5e-4)
		{
			canJump = true;
			hasJumped = true;
			anim.SetInteger("jumping", 1);
			anim.SetInteger ("landing", 0);
		}
		if(canJump){
			canJump = false;
			rb.AddForce(0, 7, 0, ForceMode.Impulse);
		}
		Vector3 running = new Vector3 (0.0f, 0.0f, moveVertical);

		if (anim.GetInteger ("running") == 1) {
			rb.AddForce (transform.forward * speed);
		}
		if (anim.GetInteger ("backing") == 1) {
			rb.AddForce (transform.forward * -7 * speed / 10);
		}
	}

	void OnTriggerStay(Collider other){

		if(other.gameObject.tag == "Platform"){
			transform.parent = other.transform;
		}
	}

	void OnTriggerExit(Collider other){
		if(other.gameObject.tag == "Platform"){
			transform.parent = null;
		}
	} 

	void OnTriggerEnter(Collider other){
		if(other.gameObject.tag == "Ground"){
			health -= 5;
			healthBar.value -= 5;
		}
	}  
}

